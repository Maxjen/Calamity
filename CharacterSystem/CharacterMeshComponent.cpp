#include "CharacterMeshComponent.h"

CharacterMeshComponent::MeshEntry::MeshEntry() :
	vertexBufferId(-1),
	indexBufferId(-1),
	vaoId(-1),
	numIndices(0),
	vertexPositions(),
	weightsByBoneName()
{}

CharacterMeshComponent::MeshEntry::~MeshEntry() {
	if (vaoId != (unsigned int)-1)
		glDeleteVertexArrays(1, &vaoId);

	if (vertexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &vertexBufferId);

	if (indexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &indexBufferId);
}


CharacterMeshComponent::CharacterMeshComponent() :
	entries(),
	textures(),
	bonesByName(nullptr),
	characterMeshShader(nullptr)
{}

bool CharacterMeshComponent::init(const string& fileName, map<string, Bone3D*>* bonesByName, CharacterMeshShader* characterMeshShader) {
	this->bonesByName = bonesByName;
	this->characterMeshShader = characterMeshShader;

	Mesh mesh;
	if (mesh.loadMeshWithWeights(fileName, bonesByName, true)) {
		entries.resize(mesh.getNumSubMeshes());
		textures.resize(mesh.getNumSubMeshes());

		for (unsigned int i = 0; i < entries.size(); ++i) {
			vector<MeshVertex> vertices;
			vector<unsigned int> indices = mesh.getIndices(i);
			entries[i].numIndices = indices.size();

			entries[i].vertexPositions = mesh.getVertexPositions(i);
			vector<glm::vec3> normals = mesh.getVertexNormals(i);
			vector<glm::vec2> texCoords = mesh.getVertexTexCoords(i);
			vector<unsigned int> boneIds = mesh.getVertexBoneIds(i);
			vector<float> boneWeights = mesh.getVertexBoneWeights(i);

			for (unsigned int j = 0; j < entries[i].vertexPositions.size(); ++j) {
				MeshVertex v;
				v.position = entries[i].vertexPositions[j];
				v.normal = normals[j];
				v.texCoord = texCoords[j];
				v.boneIds[0] = boneIds[j * 4];
				v.boneIds[1] = boneIds[j * 4 + 1];
				v.boneIds[2] = boneIds[j * 4 + 2];
				v.boneIds[3] = boneIds[j * 4 + 3];
				v.boneWeights[0] = boneWeights[j * 4];
				v.boneWeights[1] = boneWeights[j * 4 + 1];
				v.boneWeights[2] = boneWeights[j * 4 + 2];
				v.boneWeights[3] = boneWeights[j * 4 + 3];
				vertices.push_back(v);
			}

			glGenBuffers(1, &entries[i].vertexBufferId);
			glBindBuffer(GL_ARRAY_BUFFER, entries[i].vertexBufferId);
			glBufferData(GL_ARRAY_BUFFER, sizeof(MeshVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

			glGenBuffers(1, &entries[i].indexBufferId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[i].indexBufferId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * entries[i].numIndices, &indices[0], GL_STATIC_DRAW);

			entries[i].vaoId = characterMeshShader->createVertexArrayObject(entries[i].vertexBufferId, sizeof(MeshVertex), 0, (GLvoid*)(3 * sizeof(float)), (GLvoid*)(6 * sizeof(float)),
																 (GLvoid*)(8 * sizeof(float)), (GLvoid*)(8 * sizeof(float) + 4 * sizeof(unsigned int)));

			entries[i].weightsByBoneName = mesh.getWeightsByBoneName(i);

			textures[i].load(mesh.getTextureName(i).c_str());
		}
		return true;
	}
	else
		return false;

	/*meshScene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	bool result = false;

	if (meshScene) {
		//aiMatrix4x4 transformTmp = scene->mRootNode->mTransformation;
		//globalTransformInverse = glm::transpose(glm::make_mat4(&transformTmp.a1)); // maybe have to transpose
		//globalTransformInverse = glm::inverse(globalTransformInverse);

		entries.resize(meshScene->mNumMeshes);
		textures.resize(meshScene->mNumMaterials);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0; i < entries.size(); ++i) {
			const aiMesh* mesh = meshScene->mMeshes[i];
			initMesh(i, mesh);
		}

		result = initMaterials(meshScene, fileName);
	}
	else {
		printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());
	}

	return result;*/
}

void CharacterMeshComponent::render() {
	for (unsigned int i = 0; i < entries.size() ; ++i) {
		//glBindVertexArray(entries[i].vaoId);
		glBindVertexArray(entries[i].vaoId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[i].indexBufferId);

		textures[i].bind(GL_TEXTURE0);

		glDrawElements(GL_TRIANGLES, entries[i].numIndices, GL_UNSIGNED_INT, 0);

		/*if (materialIndex < textures.size() && textures[materialIndex])
			textures[materialIndex]->unbind();*/
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CharacterMeshComponent::getBoneAabb(const string& boneName, float weightThreshold, glm::vec3& aabbMin, glm::vec3& aabbMax) {
	if (bonesByName->find(boneName) != bonesByName->end() && (*bonesByName)[boneName]->hasOffsetMatrix) {
		bool bMinMaxInitialized = false;
		for (unsigned int i = 0; i < entries.size(); ++i) {
			if (entries[i].weightsByBoneName.find(boneName) != entries[i].weightsByBoneName.end()) {
				vector<VertexWeight>& weights = entries[i].weightsByBoneName[boneName];

				for (unsigned int j = 0; j < weights.size(); ++j) {
					unsigned int vertexId = weights[j].vertexId;
					float weight = weights[j].weight;

					if (weight >= weightThreshold) {
						glm::vec3 vTmp = entries[i].vertexPositions[vertexId];
						glm::vec4 v(vTmp.x, vTmp.y, vTmp.z, 1.0f);
						v = (*bonesByName)[boneName]->boneOffset * v;
						if (bMinMaxInitialized) {
							aabbMin.x = std::min(aabbMin.x, v.x);
							aabbMin.y = std::min(aabbMin.y, v.y);
							aabbMin.z = std::min(aabbMin.z, v.z);

							aabbMax.x = std::max(aabbMax.x, v.x);
							aabbMax.y = std::max(aabbMax.y, v.y);
							aabbMax.z = std::max(aabbMax.z, v.z);
						}
						else {
							aabbMin.x = v.x;
							aabbMin.y = v.y;
							aabbMin.z = v.z;
							aabbMax.x = v.x;
							aabbMax.y = v.y;
							aabbMax.z = v.z;
							bMinMaxInitialized = true;
						}
					}


					/*if (cBoneName.compare("spine") == 0)
						printf("%d\n", mesh->mBones[j]->mNumWeights);*/
				}
			}
		}
	}
	else {
		aabbMin.x = aabbMin.y = aabbMin.z = 0;
		aabbMax.x = aabbMax.y = aabbMax.z = 0;
	}
}
