#include "ClothObject.h"

ClothObject::ClothObject(Camera* camera, MeshShader* meshShader, btSoftRigidDynamicsWorld* dynamicsWorld) :
	camera(camera),
	meshShader(meshShader),
	verticesPN(),
	fixedVertices(),
	fixedVertexOffsets(),
	//mesh(),
	vertexPNBufferId(0),
	vertexTBufferId(0),
	indexBufferId(0),
	numIndices(0),
	vaoId(0),
	texture(),
	dynamicsWorld(dynamicsWorld),
	softBody(),
	bLoaded(false),
	bSelected(false)
{
	//mesh.reset(new Mesh());
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

ClothObject::~ClothObject() {
	if (vaoId != (unsigned int)-1)
		glDeleteVertexArrays(1, &vaoId);

	if (vertexPNBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &vertexPNBufferId);

	if (vertexTBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &vertexTBufferId);

	if (indexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &indexBufferId);

	if (bLoaded)
		dynamicsWorld->removeSoftBody(softBody.get());
}

bool ClothObject::loadMesh(const string& fileName) {
	if (!bLoaded) {
		Mesh mesh;
		bLoaded = mesh.loadMesh(fileName, true);
		if (bLoaded) {
			vector<glm::vec2> verticesT;
			vector<unsigned int> indices = mesh.getIndices(0);
			/*vector<int> indices;
			for (auto& it : indicesTmp) {
				indices.push_back((int)it);
			}*/
			numIndices = indices.size();

			vector<glm::vec3> positions = mesh.getVertexPositions(0);
			vector<glm::vec3> normals = mesh.getVertexNormals(0);
			vector<glm::vec2> texCoords = mesh.getVertexTexCoords(0);
			vector<glm::vec4> colors = mesh.getVertexColors(0);
			for (unsigned int i = 0; i < positions.size(); ++i) {
				positions[i] += position;
				verticesPN.push_back(VertexPN(positions[i], normals[i]));
				verticesT.push_back(texCoords[i]);
				printf("r: %f\n", colors[i].r);
				if (colors[i].r <= 0.01f) {
					fixedVertices.push_back(i);
					fixedVertexOffsets.push_back(positions[i]);
				}
			}

			glGenBuffers(1, &vertexPNBufferId);
			glBindBuffer(GL_ARRAY_BUFFER, vertexPNBufferId);
			glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * verticesPN.size(), &verticesPN[0], GL_DYNAMIC_DRAW);

			glGenBuffers(1, &vertexTBufferId);
			glBindBuffer(GL_ARRAY_BUFFER, vertexTBufferId);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * verticesT.size(), &verticesT[0], GL_STATIC_DRAW);

			glGenBuffers(1, &indexBufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);

			vaoId = meshShader->createVertexArrayObject(vertexPNBufferId, vertexTBufferId, sizeof(VertexPN), 0, (GLvoid*)(3 * sizeof(float)), 0);

			texture.load(mesh.getTextureName(0).c_str());
			//printf("texture %s\n", mesh->getTextureName(0).c_str());

			softBody.reset(btSoftBodyHelpers::CreateFromTriMesh(dynamicsWorld->getWorldInfo(), (btScalar*)&positions[0], (int*)&indices[0], indices.size() / 3, true));

			/*printf("Default\n");
			printf("totalMass: %f\n", softBody->getTotalMass());
			printf("restitution: %f\n", softBody->getRestitution());
			printf("friction: %f\n", softBody->getFriction());
			printf("anisotropicFriction: %f\n", softBody->getAnisotropicFriction());
			printf("rollingFriction: %f\n", softBody->getRollingFriction());
			printf("damping: %f\n", softBody->m_cfg.kDP);
			printf("drag: %f\n", softBody->m_cfg.kDG);
			printf("dynamicFriction: %f\n", softBody->m_cfg.kDG);
			printf("viterations: %d\n", softBody->m_cfg.viterations);
			printf("piterations: %d\n", softBody->m_cfg.piterations);*/

			/*btSoftBody::Material* material = softBody->appendMaterial();
			material->m_kLST = 0.4;

			softBody->m_cfg.kDF = 1;
			softBody->m_cfg.kSRHR_CL = 1;
			softBody->m_cfg.kSR_SPLT_CL = 0;
			softBody->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS;
			softBody->generateBendingConstraints(2, material);
			softBody->getCollisionShape()->setMargin(0.05);
			softBody->setTotalMass(50);*/

			btSoftBody::Material* material = softBody->appendMaterial();
			material->m_kLST = 0.4;

			softBody->m_cfg.kDP = 0.05f;
			softBody->m_cfg.kDG = 0.5f;
			softBody->m_cfg.kLF = 10;
			//softBody->m_cfg.kMT = 0.01f;

			softBody->m_cfg.kDF = 1;
			softBody->m_cfg.kSRHR_CL = 1;
			softBody->m_cfg.kSR_SPLT_CL = 0;
			softBody->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS;

			softBody->generateBendingConstraints(2, material);
			softBody->getCollisionShape()->setMargin(0.05);
			softBody->setTotalMass(1);


			/*for (unsigned int i = 0; i < fixedVertices.size(); ++i) {
				softBody->m_nodes[fixedVertices[i]].m_im = 0;
			}*/
			softBody->generateClusters(0);

			set<btSoftBody::Node*> fixedNodePointers;
			for (auto& it : fixedVertices)
				fixedNodePointers.insert(&(softBody->m_nodes[it]));

			for (int i = 0; i < softBody->m_clusters.size(); ++i) {
				for (int j = 0; j < softBody->m_clusters[i]->m_nodes.size(); ++j) {
					if (fixedNodePointers.find(softBody->m_clusters[i]->m_nodes[j]) != fixedNodePointers.end()) {
						softBody->m_clusters[i]->m_collide = false;
					}
				}
			}


			//softBody->setTotalMass(2.0f);
			//softBody->m_cfg.kDP = 0.1f;
			//softBody->m_cfg.kDF = 0.5f;
			/*softBody->m_cfg.viterations = 10;
			softBody->m_cfg.piterations = 10;*/
			/*softBody->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS;
			softBody->generateClusters(0);*/

			dynamicsWorld->addSoftBody(softBody.get());

			//softBody->setPose(false, true);


			/*btSoftBody* b;
			b->appendAnchor();*/
		}
	}

	return bLoaded;
}

void ClothObject::attachToRigidBody(btRigidBody* rigidBody) {
	//softBody->appendAnchor(1, rigidBody, btVector3(0, -1, 0), false);
	//softBody->m_nodes[1].m_im = 0;

	btTransform transform = rigidBody->getCenterOfMassTransform();
	glm::mat4 rigidBodyTransformInverse;
	transform.getOpenGLMatrix(glm::value_ptr(rigidBodyTransformInverse));
	rigidBodyTransformInverse = glm::inverse(rigidBodyTransformInverse);

	for (unsigned int i = 0; i < fixedVertices.size(); ++i) {
		glm::vec4 v = glm::vec4(fixedVertexOffsets[i].x, fixedVertexOffsets[i].y - 4, fixedVertexOffsets[i].z, 1.0f);
		v = rigidBodyTransformInverse * v;
		fixedVertexOffsets[i] = glm::vec3(v.x, v.y, v.z);
		softBody->m_nodes[fixedVertices[i]].m_im = 0;
	}
}

int ClothObject::getObjectType() {
	return CLOTH_OBJECT;
}

void ClothObject::setPosition(float posX, float posY, float posZ) {
	position = glm::vec3(posX, posY, posZ);

	/*if (bLoaded) {
		glm::vec3 aabbMin = mesh->getAabbMin();
		glm::vec3 aabbMax = mesh->getAabbMax();
		glm::vec3 aabbPos = (aabbMax + aabbMin) * 0.5f;
		aabbPos.x *= scale.x;
		aabbPos.y *= scale.y;
		aabbPos.z *= scale.z;
		aabbPos += position;*/


		/*btTransform transform = rigidBody->getCenterOfMassTransform();
		transform.setOrigin(btVector3(aabbPos.x, aabbPos.y, aabbPos.z));
		//transform.setOrigin(btVector3(posX, posY, posZ));
		rigidBody->setCenterOfMassTransform(transform);*/

		//dynamicsWorld->stepSimulation(1 / 60.f, 10);
	//}
}

void ClothObject::setScale(float scaleX, float scaleY, float scaleZ) {
	//scale = glm::vec3(scaleX, scaleY, scaleZ);

	/*if (bLoaded) {
		dynamicsWorld->removeRigidBody(rigidBody.get());
		delete rigidBody->getMotionState();

		loadCollisionObject();
	}*/
}

void ClothObject::toggleSelection() {
	bSelected = !bSelected;
}

void ClothObject::update() {
	verticesPN.clear();
	for (int i = 0; i < softBody->m_nodes.size(); ++i) {
		glm::vec3 position = glm::vec3(softBody->m_nodes[i].m_x.x(),
									   softBody->m_nodes[i].m_x.y(),
									   softBody->m_nodes[i].m_x.z());
		glm::vec3 normal = glm::vec3(softBody->m_nodes[i].m_n.x(),
									 softBody->m_nodes[i].m_n.y(),
									 softBody->m_nodes[i].m_n.z());
		verticesPN.push_back(VertexPN(position, normal));
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexPNBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * verticesPN.size(), &verticesPN[0], GL_DYNAMIC_DRAW);
}

void ClothObject::setAPosition(glm::vec3 position) {
	//softBody->m_nodes[1].m_x = btVector3(position.x, position.y, position.z);

	//printf("%u\n", fixedVertices.size());

	for (unsigned int i = 0; i < fixedVertices.size(); ++i) {
		softBody->m_nodes[fixedVertices[i]].m_x = btVector3(position.x + fixedVertexOffsets[i].x, position.y + fixedVertexOffsets[i].y, position.z + fixedVertexOffsets[i].z);
	}
}

void ClothObject::applyMatrix(glm::mat4 matrix) {
	for (unsigned int i = 0; i < fixedVertexOffsets.size(); ++i) {
		glm::vec4 v = glm::vec4(fixedVertexOffsets[i].x, fixedVertexOffsets[i].y, fixedVertexOffsets[i].z, 1.0f);
		v = matrix * v;
		softBody->m_nodes[fixedVertices[i]].m_x = btVector3(v.x, v.y, v.z);
	}
}

void ClothObject::render() {
	if (bLoaded) {
		/*glm::mat4 modelMatrix = glm::translate(glm::mat4(), position);
		modelMatrix = glm::scale(modelMatrix, scale);

		glm::mat4 modelViewMatrix = camera->getViewMatrix() * modelMatrix;

		glm::mat4 modelInverseTranspose = glm::inverse(modelMatrix);
		modelInverseTranspose = glm::transpose(modelInverseTranspose);*/

		glm::mat4 modelViewMatrix = camera->getViewMatrix();

		/*glm::mat4 modelInverseTranspose = glm::inverse(modelMatrix);
		modelInverseTranspose = glm::transpose(modelInverseTranspose);*/

		meshShader->bind();
		meshShader->setModelViewMatrix(modelViewMatrix);
		meshShader->setModelMatrix(glm::mat4());
		meshShader->setModelInverseTranspose(glm::mat4());


		if (bSelected) {
			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);

			// Render the mesh into the stencil buffer.

			glEnable(GL_STENCIL_TEST);

			glStencilFunc(GL_ALWAYS, 1, -1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			renderInternal();

			// Render the thick wireframe version.

			glStencilFunc(GL_NOTEQUAL, 1, -1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			glLineWidth(3);
			glPolygonMode(GL_FRONT, GL_LINE);

			meshShader->setRenderBlack(true);
			renderInternal();
			meshShader->setRenderBlack(false);

			glPolygonMode(GL_FRONT, GL_FILL);

			glDisable(GL_STENCIL_TEST);
		}
		else
			renderInternal();

		meshShader->unbind();
	}
}

/*void ClothObject::loadCollisionObject(vector<glm::vec3> positions, vector<unsigned int> indices) {
	glm::vec3 aabbMin = mesh->getAabbMin();
	glm::vec3 aabbMax = mesh->getAabbMax();
	glm::vec3 aabbPos = (aabbMax + aabbMin) * 0.5f;
	aabbPos.x *= scale.x;
	aabbPos.y *= scale.y;
	aabbPos.z *= scale.z;
	aabbPos += position;
	float dx = (aabbMax.x - aabbMin.x) / 2 * scale.x;
	float dy = (aabbMax.y - aabbMin.y) / 2 * scale.y;
	float dz = (aabbMax.z - aabbMin.z) / 2 * scale.z;*/

	/*collisionShape.reset(new btBoxShape(btVector3(dx, dy, dz)));
	//collisionShape.reset(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)));
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(aabbPos.x, aabbPos.y, aabbPos.z)));
	//btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, motionState, collisionShape.get(), btVector3(0, 0, 0));
	rigidBody.reset(new btRigidBody(constructionInfo));
	dynamicsWorld->addRigidBody(rigidBody.get());
	rigidBody->setUserPointer((void*)this);*/

	/*float s = 4;
	float h = 20;
	btSoftBody* softBody = btSoftBodyHelpers::CreatePatch(dynamicsWorld->getWorldInfo(),
														  btVector3(-s, h, -s), btVector3(s, h, -s), btVector3(-s, h, s), btVector3(s, h, s), 10, 10, 0, true);
	dynamicsWorld->addSoftBody(softBody);


}*/

void ClothObject::renderInternal() {
	//glDisable(GL_CULL_FACE);
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	texture.bind(GL_TEXTURE0);
	glCullFace(GL_FRONT);
	meshShader->setInvertNormals(true);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	meshShader->setInvertNormals(false);
	glCullFace(GL_BACK);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	texture.unbind();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//glEnable(GL_CULL_FACE);
}
