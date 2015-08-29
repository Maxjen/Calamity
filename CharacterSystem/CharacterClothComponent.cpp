#include "CharacterClothComponent.h"

CharacterClothComponent::CharacterClothComponent() :
	maxDistanceScale(),
	clothVertices(),
	fixedVertices(),
	vertexPNBufferId(-1),
	vertexTBufferId(-1),
	indexBufferId(-1),
	numIndices(0),
	vaoId(-1),
	texture(),

	//bonesByName(nullptr),
	bonesById(),
	meshShader(nullptr),

	dynamicsWorld(nullptr),
	softBody()
{}

CharacterClothComponent::~CharacterClothComponent() {
	if (vaoId != (unsigned int)-1)
		glDeleteVertexArrays(1, &vaoId);

	if (vertexPNBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &vertexPNBufferId);

	if (vertexTBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &vertexTBufferId);

	if (indexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &indexBufferId);

	if (softBody)
		dynamicsWorld->removeSoftBody(softBody.get());
}

bool CharacterClothComponent::init(const string& fileName, map<string, Bone3D*>* bonesByName, MeshShader* meshShader, CharacterEditor3DProperties* characterEditor3DProperties, btSoftRigidDynamicsWorld* dynamicsWorld) {
	//this->bonesByName = bonesByName;
	this->meshShader = meshShader;
	this->dynamicsWorld = dynamicsWorld;

	for (auto& it : *bonesByName)
		bonesById.push_back(it.second);

	Mesh mesh;
	if (mesh.loadMeshWithWeights(fileName, bonesByName, true)) {
		vector<float> verticesPN;
		vector<glm::vec2> verticesT;

		vector<unsigned int> indices = mesh.getIndices(0);
		numIndices = indices.size();

		vector<glm::vec3> positions = mesh.getVertexPositions(0);
		vector<glm::vec3> normals = mesh.getVertexNormals(0);
		vector<glm::vec2> texCoords = mesh.getVertexTexCoords(0);
		vector<unsigned int> boneIds = mesh.getVertexBoneIds(0);
		vector<float> boneWeights = mesh.getVertexBoneWeights(0);
		vector<glm::vec4> colors = mesh.getVertexColors(0);

		vector<glm::vec3> initialPositions;

		for (unsigned int i = 0; i < positions.size(); ++i) {
			ClothVertex v;
			v.position = positions[i];
			v.boneIds[0] = boneIds[i * 4];
			v.boneIds[1] = boneIds[i * 4 + 1];
			v.boneIds[2] = boneIds[i * 4 + 2];
			v.boneIds[3] = boneIds[i * 4 + 3];
			v.boneWeights[0] = boneWeights[i * 4];
			v.boneWeights[1] = boneWeights[i * 4 + 1];
			v.boneWeights[2] = boneWeights[i * 4 + 2];
			v.boneWeights[3] = boneWeights[i * 4 + 3];
			v.maxDistance = colors[i].r;
			if (v.maxDistance <= 0.01f)
				fixedVertices.push_back(i);
			clothVertices.push_back(v);

			verticesPN.push_back(positions[i].x);
			verticesPN.push_back(positions[i].y);
			verticesPN.push_back(positions[i].z);
			verticesPN.push_back(normals[i].x);
			verticesPN.push_back(normals[i].y);
			verticesPN.push_back(normals[i].z);

			verticesT.push_back(texCoords[i]);


			glm::mat4 boneTransform = bonesById[v.boneIds[0]]->finalTransformation * v.boneWeights[0] +
					bonesById[v.boneIds[1]]->finalTransformation * v.boneWeights[1] +
					bonesById[v.boneIds[2]]->finalTransformation * v.boneWeights[2] +
					bonesById[v.boneIds[3]]->finalTransformation * v.boneWeights[3];

			initialPositions.push_back(glm::vec3(boneTransform * glm::vec4(v.position, 1.0f)));
		}

		glGenBuffers(1, &vertexPNBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPNBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * clothVertices.size(), &verticesPN[0], GL_DYNAMIC_DRAW);

		glGenBuffers(1, &vertexTBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexTBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * verticesT.size(), &verticesT[0], GL_STATIC_DRAW);

		glGenBuffers(1, &indexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);

		vaoId = meshShader->createVertexArrayObject(vertexPNBufferId, vertexTBufferId, sizeof(float) * 6, 0, (GLvoid*)(3 * sizeof(float)),  0);

		//entries[i].weightsByBoneName = mesh.getWeightsByBoneName(i);

		texture.load(mesh.getTextureName(0).c_str());


		maxDistanceScale = characterEditor3DProperties->getMaxDistance();

		softBody.reset(btSoftBodyHelpers::CreateFromTriMesh(dynamicsWorld->getWorldInfo(), (btScalar*)&initialPositions[0], (int*)&indices[0], indices.size() / 3, true));

		btSoftBody::Material* material = softBody->appendMaterial();
		material->m_kLST = characterEditor3DProperties->getLinStiffness();
		material->m_kAST = characterEditor3DProperties->getAngStiffness();

		softBody->m_cfg.kDP = characterEditor3DProperties->getDamping();
		softBody->m_cfg.kDG = characterEditor3DProperties->getDrag();
		softBody->m_cfg.kDF = characterEditor3DProperties->getDynamicFriction();
		softBody->m_cfg.kLF = characterEditor3DProperties->getLift();

		//softBody->m_cfg.kDP = 0.05f;
		//softBody->m_cfg.kDG = 0.5f;
		//softBody->m_cfg.kLF = 10;
		//softBody->m_cfg.kMT = 0.01f;

		//softBody->m_cfg.kDF = 1;
		softBody->m_cfg.kSRHR_CL = 1;
		softBody->m_cfg.kSR_SPLT_CL = 0;
		softBody->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS;

		softBody->generateBendingConstraints(2, material);
		softBody->getCollisionShape()->setMargin(0.05);
		//softBody->setTotalMass(1);

		softBody->setTotalMass(characterEditor3DProperties->getTotalMass());

		for (unsigned int i = 0; i < fixedVertices.size(); ++i) {
			softBody->m_nodes[fixedVertices[i]].m_im = 0;
		}
		softBody->generateClusters(0);

		// TODO: figure out why this is not done by generateClusters
		/*set<btSoftBody::Node*> fixedNodePointers;
		for (auto& it : fixedVertices)
			fixedNodePointers.insert(&(softBody->m_nodes[it]));

		for (int i = 0; i < softBody->m_clusters.size(); ++i) {
			for (int j = 0; j < softBody->m_clusters[i]->m_nodes.size(); ++j) {
				if (fixedNodePointers.find(softBody->m_clusters[i]->m_nodes[j]) != fixedNodePointers.end()) {
					softBody->m_clusters[i]->m_collide = false;
				}
			}
		}*/

		dynamicsWorld->addSoftBody(softBody.get());





		return true;
	}
	else
		return false;
}

void CharacterClothComponent::setMaxDistanceScale(float maxDistanceScale) {
	this->maxDistanceScale = maxDistanceScale;
}

void CharacterClothComponent::render() {
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	texture.bind(GL_TEXTURE0);

	glCullFace(GL_FRONT);
	meshShader->setInvertNormals(true);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	glCullFace(GL_BACK);
	meshShader->setInvertNormals(false);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CharacterClothComponent::update() {
	vector<float> verticesPN;

	/*for (auto& it : fixedVertices) {
		glm::mat4 boneTransform = bonesById[clothVertices[it].boneIds[0]]->finalTransformation * clothVertices[it].boneWeights[0] +
				bonesById[clothVertices[it].boneIds[1]]->finalTransformation * clothVertices[it].boneWeights[1] +
				bonesById[clothVertices[it].boneIds[2]]->finalTransformation * clothVertices[it].boneWeights[2] +
				bonesById[clothVertices[it].boneIds[3]]->finalTransformation * clothVertices[it].boneWeights[3];

		glm::vec3 position = glm::vec3(boneTransform * glm::vec4(clothVertices[it].position, 1.0f));

		softBody->m_nodes[it].m_x = btVector3(position.x, position.y, position.z);
	}*/

	for (unsigned int i = 0; i < clothVertices.size(); ++i) {
		glm::mat4 boneTransform = bonesById[clothVertices[i].boneIds[0]]->finalTransformation * clothVertices[i].boneWeights[0] +
				bonesById[clothVertices[i].boneIds[1]]->finalTransformation * clothVertices[i].boneWeights[1] +
				bonesById[clothVertices[i].boneIds[2]]->finalTransformation * clothVertices[i].boneWeights[2] +
				bonesById[clothVertices[i].boneIds[3]]->finalTransformation * clothVertices[i].boneWeights[3];

		glm::vec3 keyPosition = glm::vec3(boneTransform * glm::vec4(clothVertices[i].position, 1.0f));

		if (clothVertices[i].maxDistance  <= 0.01f)
			softBody->m_nodes[i].m_x = btVector3(keyPosition.x, keyPosition.y, keyPosition.z);
		else {
			glm::vec3 d = glm::vec3(softBody->m_nodes[i].m_x.getX(), softBody->m_nodes[i].m_x.getY(), softBody->m_nodes[i].m_x.getZ());
			d -= keyPosition;
			float distance = glm::length(d);
			if (distance > clothVertices[i].maxDistance * maxDistanceScale) {
				float scale = clothVertices[i].maxDistance * maxDistanceScale / distance;
				d *= scale;
				glm::vec3 newPosition = keyPosition + d;
				softBody->m_nodes[i].m_x = btVector3(newPosition.x, newPosition.y, newPosition.z);
			}
		}
	}

	// this method seems to only update normals
	softBody->integrateMotion();

	for (int i = 0; i < softBody->m_nodes.size(); ++i) {
		glm::vec3 position = glm::vec3(softBody->m_nodes[i].m_x.x(),
									   softBody->m_nodes[i].m_x.y(),
									   softBody->m_nodes[i].m_x.z());
		glm::vec3 normal = glm::vec3(softBody->m_nodes[i].m_n.x(),
									 softBody->m_nodes[i].m_n.y(),
									 softBody->m_nodes[i].m_n.z());
		verticesPN.push_back(position.x);
		verticesPN.push_back(position.y);
		verticesPN.push_back(position.z);
		verticesPN.push_back(normal.x);
		verticesPN.push_back(normal.y);
		verticesPN.push_back(normal.z);
	}

	/*for (auto& it : clothVertices) {
		//printf("%s %s %s %s\n", bonesById[it.boneIds[0]]->boneName.c_str(), bonesById[it.boneIds[1]]->boneName.c_str(), bonesById[it.boneIds[2]]->boneName.c_str(), bonesById[it.boneIds[3]]->boneName.c_str());
		//printf("%u %u %u %u\n", it.boneIds[0], it.boneIds[1], it.boneIds[2], it.boneIds[3]);
		//printf("%f %f %f %f\n", it.boneWeights[0], it.boneWeights[1], it.boneWeights[2], it.boneWeights[3]);

		glm::mat4 boneTransform = bonesById[it.boneIds[0]]->finalTransformation * it.boneWeights[0] +
				bonesById[it.boneIds[1]]->finalTransformation * it.boneWeights[1] +
				bonesById[it.boneIds[2]]->finalTransformation * it.boneWeights[2] +
				bonesById[it.boneIds[3]]->finalTransformation * it.boneWeights[3];

		glm::vec3 position = glm::vec3(boneTransform * glm::vec4(it.position, 1.0f));

		//if (position.x == 0 && position.y == 0 && position.z == 0) {
			//printf("%s %s %s %s\n", bonesById[it.boneIds[0]]->boneName.c_str(), bonesById[it.boneIds[1]]->boneName.c_str(), bonesById[it.boneIds[2]]->boneName.c_str(), bonesById[it.boneIds[3]]->boneName.c_str());
			//printf("%u %u %u %u\n", it.boneIds[0], it.boneIds[1], it.boneIds[2], it.boneIds[3]);
			//printf("%f %f %f %f\n", it.boneWeights[0], it.boneWeights[1], it.boneWeights[2], it.boneWeights[3]);
		//}

		verticesPN.push_back(position.x);
		verticesPN.push_back(position.y);
		verticesPN.push_back(position.z);
		verticesPN.push_back(position.x);
		verticesPN.push_back(position.y);
		verticesPN.push_back(position.z);
		//verticesPN.push_back(normals[i].x);
		//verticesPN.push_back(normals[i].y);
		//verticesPN.push_back(normals[i].z);
	}*/

	glBindBuffer(GL_ARRAY_BUFFER, vertexPNBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * clothVertices.size(), &verticesPN[0], GL_DYNAMIC_DRAW);
}
