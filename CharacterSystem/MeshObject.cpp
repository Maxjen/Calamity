#include "MeshObject.h"

MeshObject::MeshObject(Camera* camera, MeshShader* meshShader, btSoftRigidDynamicsWorld* dynamicsWorld) :
	camera(camera),
	meshShader(meshShader),
	//mesh(),
	vertexBufferId(0),
	indexBufferId(0),
	numIndices(0),
	vaoId(0),
	texture(),
	aabbMin(),
	aabbMax(),
	dynamicsWorld(dynamicsWorld),
	collisionShape(),
	rigidBody(),
	bLoaded(false),
	bSelected(false)
{
	//mesh.reset(new Mesh());
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

MeshObject::~MeshObject() {
	if (vaoId != (unsigned int)-1)
		glDeleteVertexArrays(1, &vaoId);

	if (vertexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &vertexBufferId);

	if (indexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &indexBufferId);

	if (bLoaded) {
		dynamicsWorld->removeRigidBody(rigidBody.get());
		delete rigidBody->getMotionState();
	}
}

bool MeshObject::loadMesh(const string& fileName) {
	if (!bLoaded) {
		Mesh mesh;
		bLoaded = mesh.loadMesh(fileName, true);
		if (bLoaded) {
			vector<Vertex> vertices;
			vector<unsigned int> indices = mesh.getIndices(0);
			numIndices = indices.size();

			vector<glm::vec3> positions = mesh.getVertexPositions(0);
			vector<glm::vec3> normals = mesh.getVertexNormals(0);
			vector<glm::vec2> texCoords = mesh.getVertexTexCoords(0);
			for (unsigned int i = 0; i < positions.size(); ++i)
				vertices.push_back(Vertex(positions[i], normals[i], texCoords[i]));

			glGenBuffers(1, &vertexBufferId);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &indexBufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);

			vaoId = meshShader->createVertexArrayObject(vertexBufferId, sizeof(Vertex), 0, (GLvoid*)(3 * sizeof(float)), (GLvoid*)(6 * sizeof(float)));

			texture.load(mesh.getTextureName(0).c_str());
			//printf("texture %s\n", mesh->getTextureName(0).c_str());

			aabbMin = mesh.getAabbMin();
			aabbMax = mesh.getAabbMax();

			loadCollisionObject();
		}
	}

	return bLoaded;
}

btRigidBody* MeshObject::getRigidBody() {
	return rigidBody.get();
}

int MeshObject::getObjectType() {
	return MESH_OBJECT;
}

void MeshObject::setPosition(float posX, float posY, float posZ) {
	//glm::vec3 deltaPosition = glm::vec3(posX, posY, posZ) - position;
	position = glm::vec3(posX, posY, posZ);

	if (bLoaded) {
		/*glm::vec3 aabbMin = mesh->getAabbMin();
		glm::vec3 aabbMax = mesh->getAabbMax();*/
		glm::vec3 aabbPos = (aabbMax + aabbMin) * 0.5f;
		aabbPos.x *= scale.x;
		aabbPos.y *= scale.y;
		aabbPos.z *= scale.z;
		aabbPos += position;

		/*btRigidBody* r;
		r->getMotionState()->setWorldTransform();*/

		/*btTransform transform;
		rigidBody->getMotionState()->getWorldTransform(transform);
		transform.setOrigin(btVector3(posX, posY, posZ));
		rigidBody->getMotionState()->setWorldTransform(transform);*/




		btTransform transform = rigidBody->getCenterOfMassTransform();
		transform.setOrigin(btVector3(aabbPos.x, aabbPos.y, aabbPos.z));
		//transform.setOrigin(btVector3(posX, posY, posZ));
		rigidBody->setCenterOfMassTransform(transform);


		/*rigidBody->translate(btVector3(deltaPosition.x, deltaPosition.y, deltaPosition.z));
		position += deltaPosition;*/




		//dynamicsWorld->stepSimulation(1 / 60.f, 10);
	}
}

void MeshObject::setScale(float scaleX, float scaleY, float scaleZ) {
	scale = glm::vec3(scaleX, scaleY, scaleZ);

	/*if (bLoaded) {
		dynamicsWorld->removeRigidBody(rigidBody.get());
		delete rigidBody->getMotionState();

		loadCollisionObject();
	}*/
}

void MeshObject::toggleSelection() {
	bSelected = !bSelected;
}

void MeshObject::render() {
	if (bLoaded) {
		glm::mat4 modelMatrix = glm::translate(glm::mat4(), position);
		modelMatrix = glm::scale(modelMatrix, scale);

		glm::mat4 modelViewMatrix = camera->getViewMatrix() * modelMatrix;

		glm::mat4 modelInverseTranspose = glm::inverse(modelMatrix);
		modelInverseTranspose = glm::transpose(modelInverseTranspose);

		meshShader->bind();
		meshShader->setModelViewMatrix(modelViewMatrix);
		meshShader->setModelMatrix(modelMatrix);
		meshShader->setModelInverseTranspose(modelInverseTranspose);


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

void MeshObject::loadCollisionObject() {
	/*glm::vec3 aabbMin = mesh->getAabbMin();
	glm::vec3 aabbMax = mesh->getAabbMax();*/
	glm::vec3 aabbPos = (aabbMax + aabbMin) * 0.5f;
	aabbPos.x *= scale.x;
	aabbPos.y *= scale.y;
	aabbPos.z *= scale.z;
	aabbPos += position;
	float dx = (aabbMax.x - aabbMin.x) / 2 * scale.x;
	float dy = (aabbMax.y - aabbMin.y) / 2 * scale.y;
	float dz = (aabbMax.z - aabbMin.z) / 2 * scale.z;

	collisionShape.reset(new btBoxShape(btVector3(dx, dy, dz)));
	//collisionShape.reset(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)));
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(aabbPos.x, aabbPos.y, aabbPos.z)));
	//btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, motionState, collisionShape.get(), btVector3(0, 0, 0));
	rigidBody.reset(new btRigidBody(constructionInfo));
	dynamicsWorld->addRigidBody(rigidBody.get());
	rigidBody->setUserPointer((void*)this);
}

void MeshObject::renderInternal() {
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	texture.bind(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	texture.unbind();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
