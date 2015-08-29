#include "Mesh.h"

Bone3D::Bone3D() :
	boneName(),
	boneId(0),
	hasOffsetMatrix(false),
	boneOffset(),
	finalTransformation(),
	nodeTransformation(),
	globalTransformation(),
	scaling(),
	rotation(),
	translation(),
	parent(nullptr),
	children(),
	bHasRigidBody(false),
	dynamicsWorld(nullptr),
	collisionShape(),
	rigidBody()
{}

Bone3D::~Bone3D() {
	if (bHasRigidBody) {
		dynamicsWorld->removeRigidBody(rigidBody.get());
		delete rigidBody->getMotionState();
	}

	for (auto& it : children)
		delete it;
}

void Bone3D::addRigidBody(btDiscreteDynamicsWorld* dynamicsWorld, float radius, float height, glm::vec3 offset) {
	bHasRigidBody = true;
	this->dynamicsWorld = dynamicsWorld;
	rigidBodyOffset = offset;
	float heightCylinder = std::max(0.0f, height - 2 * radius);
	collisionShape.reset(new btCapsuleShape(radius, heightCylinder));
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, motionState, collisionShape.get(), btVector3(0, 0, 0));
	rigidBody.reset(new btRigidBody(constructionInfo));
	//rigidBody->setContactProcessingThreshold(BT_LARGE_FLOAT);
	dynamicsWorld->addRigidBody(rigidBody.get());
	//rigidBody->setUserPointer((void*)this);

	glm::mat4 rigidBodyTransformation = glm::translate(globalTransformation, rigidBodyOffset);
	btTransform transform;
	transform.setFromOpenGLMatrix(glm::value_ptr(rigidBodyTransformation));
	rigidBody->setCenterOfMassTransform(transform);
}

Animation::Animation() :
	ticksPerSecond(0),
	duration(0),
	boneAnimations()
{}

void Animation::init(aiAnimation* animation, map<std::string, Bone3D*>* bonesByName) {
	ticksPerSecond = (float)(animation->mTicksPerSecond != 0 ? animation->mTicksPerSecond : 25.0f);
	duration = (float)animation->mDuration;

	for (unsigned int i = 0; i < animation->mNumChannels; ++i) {
		const aiNodeAnim* nodeAnim = animation->mChannels[i];

		string boneName(nodeAnim->mNodeName.data);
		if (bonesByName->find(boneName) != bonesByName->end()) {
			BoneAnimation& bA = boneAnimations[boneName];

			for (unsigned int j = 0; j < nodeAnim->mNumScalingKeys; ++j) {
				ScalingKey k;
				k.time = nodeAnim->mScalingKeys[j].mTime;
				k.scale = glm::vec3(nodeAnim->mScalingKeys[j].mValue.x, nodeAnim->mScalingKeys[j].mValue.y, nodeAnim->mScalingKeys[j].mValue.z);
				bA.scalingKeys.push_back(k);
			}

			for (unsigned int j = 0; j < nodeAnim->mNumRotationKeys; ++j) {
				RotationKey k;
				k.time = nodeAnim->mRotationKeys[j].mTime;
				aiQuaternion q;
				k.rotation = glm::quat(nodeAnim->mRotationKeys[j].mValue.w, nodeAnim->mRotationKeys[j].mValue.x, nodeAnim->mRotationKeys[j].mValue.y, nodeAnim->mRotationKeys[j].mValue.z);
				bA.rotationKeys.push_back(k);
			}

			for (unsigned int j = 0; j < nodeAnim->mNumPositionKeys; ++j) {
				PositionKey k;
				k.time = nodeAnim->mPositionKeys[j].mTime;
				k.position = glm::vec3(nodeAnim->mPositionKeys[j].mValue.x, nodeAnim->mPositionKeys[j].mValue.y, nodeAnim->mPositionKeys[j].mValue.z);
				bA.positionKeys.push_back(k);
			}

			//printf("%s\nscalingKeys %u\nrotationKeys %u\npositionKeys%u\n", boneName.c_str(), nodeAnim->mNumScalingKeys, nodeAnim->mNumRotationKeys, nodeAnim->mNumPositionKeys);
			//printf("%s\nscalingKeys %u\nrotationKeys %u\npositionKeys%u\n", boneName.c_str(), boneAnimations[boneName].scalingKeys.size(), boneAnimations[boneName].rotationKeys.size(), boneAnimations[boneName].positionKeys.size());
		}
	}
}

float Animation::getTicksPerSecond() {
	return ticksPerSecond;
}

float Animation::getDuration() {
	return duration;
}

bool Animation::hasBoneAnimation(const std::string &boneName) {
	return boneAnimations.find(boneName) != boneAnimations.end();
}

glm::vec3 Animation::interpolateScaling(float animationTime, const std::string& boneName) {
	BoneAnimation& anim = boneAnimations[boneName];

	assert(anim.scalingKeys.size() > 0);

	unsigned int keyIndex = (unsigned int)-1;
	for (unsigned int i = 0; i < anim.scalingKeys.size() - 1; ++i) {
		if (animationTime < (float)anim.scalingKeys[i + 1].time) {
			keyIndex = i;
			break;
		}
	}
	if (keyIndex == (unsigned int)-1) {
		printf("Coudn't find scaling key index for animation time %f\n", animationTime);
		return glm::vec3(0, 0, 0);
	}

	if (anim.scalingKeys.size() == 1) {
		return anim.scalingKeys[0].scale;
	}

	unsigned int nextKeyIndex = keyIndex + 1;
	assert(nextKeyIndex < anim.scalingKeys.size());
	float deltaTime = (float)(anim.scalingKeys[nextKeyIndex].time - anim.scalingKeys[keyIndex].time);
	float factor = (animationTime - (float)anim.scalingKeys[keyIndex].time) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = anim.scalingKeys[keyIndex].scale;
	const glm::vec3& end = anim.scalingKeys[nextKeyIndex].scale;
	glm::vec3 delta = end - start;
	return start + factor * delta;
}

glm::quat Animation::interpolateRotation(float animationTime, const std::string& boneName) {
	BoneAnimation& anim = boneAnimations[boneName];

	assert(anim.rotationKeys.size() > 0);

	unsigned int keyIndex = (unsigned int)-1;
	for (unsigned int i = 0; i < anim.rotationKeys.size() - 1; ++i) {
		if (animationTime < (float)anim.rotationKeys[i + 1].time) {
			keyIndex = i;
			break;
		}
	}
	if (keyIndex == (unsigned int)-1) {
		printf("Coudn't find rotation key index for animation time %f\n", animationTime);
		return glm::quat(0, 0, 0, 0);
	}

	if (anim.rotationKeys.size() == 1) {
		return anim.rotationKeys[0].rotation;
	}

	unsigned int nextKeyIndex = keyIndex + 1;
	assert(nextKeyIndex < anim.rotationKeys.size());
	float deltaTime = (float)(anim.rotationKeys[nextKeyIndex].time - anim.rotationKeys[keyIndex].time);
	float factor = (animationTime - (float)anim.rotationKeys[keyIndex].time) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::quat& start = anim.rotationKeys[keyIndex].rotation;
	const glm::quat& end = anim.rotationKeys[nextKeyIndex].rotation;
	return glm::slerp(start, end, factor);
}

glm::vec3 Animation::interpolatePosition(float animationTime, const std::string& boneName) {
	BoneAnimation& anim = boneAnimations[boneName];

	assert(anim.positionKeys.size() > 0);

	unsigned int keyIndex = (unsigned int)-1;
	for (unsigned int i = 0; i < anim.positionKeys.size() - 1; ++i) {
		if (animationTime < (float)anim.positionKeys[i + 1].time) {
			keyIndex = i;
			break;
		}
	}
	if (keyIndex == (unsigned int)-1) {
		printf("Coudn't find position key index for animation time %f\n", animationTime);
		return glm::vec3(0, 0, 0);
	}

	if (anim.positionKeys.size() == 1) {
		return anim.positionKeys[0].position;
	}

	unsigned int nextKeyIndex = keyIndex + 1;
	assert(nextKeyIndex < anim.positionKeys.size());
	float deltaTime = (float)(anim.positionKeys[nextKeyIndex].time - anim.positionKeys[keyIndex].time);
	float factor = (animationTime - (float)anim.positionKeys[keyIndex].time) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const glm::vec3& start = anim.positionKeys[keyIndex].position;
	const glm::vec3& end = anim.positionKeys[nextKeyIndex].position;
	glm::vec3 delta = end - start;
	return start + factor * delta;
}

Mesh::Mesh() :
	boneHierarchy(),
	bDeleteBoneHierarchy(true),
	bonesByName(),
	animations(),
	aabbMin(0, 0, 0),
	aabbMax(0, 0, 0),
	entries()
{}

Mesh::~Mesh() {
	if (bDeleteBoneHierarchy)
		delete boneHierarchy;
}

bool Mesh::loadSkeleton(const string& fileName) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	//scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (scene) {
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
				string boneName(mesh->mBones[i]->mName.data);

				if (bonesByName.find(boneName) == bonesByName.end()) {
					Bone3D* b = new Bone3D();
					b->hasOffsetMatrix = true;
					aiMatrix4x4 offsetMatrixTmp = mesh->mBones[i]->mOffsetMatrix;
					b->boneOffset = glm::transpose(glm::make_mat4(&offsetMatrixTmp.a1));
					bonesByName[boneName] = b;
				}
			}
		}

		aiNode* rootBone = getRootBone(scene->mRootNode);
		boneHierarchy = createBoneHierarchy(rootBone, nullptr);

		unsigned int i = 0;
		for (auto& it: bonesByName) {
			it.second->boneId = i;
			++i;
		}

		for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
			string animationName(scene->mAnimations[i]->mName.data);
			printf("%s\n", animationName.c_str());
			if (animations.find(animationName) == animations.end())
				animations[animationName].init(scene->mAnimations[i], &bonesByName);		}

		return true;
	}
	else {
		printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());
		return false;
	}
}

Bone3D* Mesh::extractBoneHierarchy() {
	bDeleteBoneHierarchy = false;
	return boneHierarchy;
}

map<string, Bone3D*> Mesh::getBonesByName() {
	return bonesByName;
}

map<string, Animation> Mesh::getAnimations() {
	return animations;
}

aiNode* Mesh::getRootBone(aiNode* node) {
	string boneName(node->mName.data);
	if (bonesByName.find(boneName) != bonesByName.end())
		return node;

	aiNode* result = nullptr;
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		result = getRootBone(node->mChildren[i]);
		if (result != nullptr)
			return result;
	}

	return nullptr;
}

Bone3D* Mesh::createBoneHierarchy(aiNode* node, Bone3D* parent) {
	string boneName(node->mName.data);
	//node->mTransformation;

	Bone3D* result;
	if (bonesByName.find(boneName) != bonesByName.end())
		result = bonesByName[boneName];
	else {
		result = new Bone3D();
		printf("newBone: %s\n", boneName.c_str());
		bonesByName[boneName] = result;
	}

	result->nodeTransformation = glm::transpose(glm::make_mat4(&node->mTransformation.a1));
	if (parent) {
		result->globalTransformation = parent->globalTransformation * result->nodeTransformation;
	}
	else
		result->globalTransformation = result->nodeTransformation;

	/*glm::mat4 nodeTransformation = result->nodeTransformation;
	printf("nodeTransformation:\n");
	printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",
		   nodeTransformation[0][0], nodeTransformation[0][1], nodeTransformation[0][2], nodeTransformation[0][3],
			nodeTransformation[1][0], nodeTransformation[1][1], nodeTransformation[1][2], nodeTransformation[1][3],
			nodeTransformation[2][0], nodeTransformation[2][1], nodeTransformation[2][2], nodeTransformation[2][3],
			nodeTransformation[3][0], nodeTransformation[3][1], nodeTransformation[3][2], nodeTransformation[3][3]);*/

	result->boneName = boneName;
	result->parent = parent;
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
		result->children.push_back(createBoneHierarchy(node->mChildren[i], result));
	return result;
}

bool Mesh::loadMesh(const std::string& fileName, bool triangulate) {
	Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	const aiScene* scene;
	if (triangulate)
		scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	else
		scene = importer.ReadFile(fileName.c_str(), aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (scene) {
		entries.resize(scene->mNumMeshes);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0; i < entries.size(); ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			initMesh(i, mesh, scene, fileName);
		}
		return true;
	}
	else {
		printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());
		return false;
	}
}

bool Mesh::loadMeshWithWeights(const std::string& fileName, map<std::string, Bone3D*>* bonesByName, bool triangulate) {
	Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	const aiScene* scene;
	if (triangulate)
		scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
	else
		scene = importer.ReadFile(fileName.c_str(), aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (scene) {
		entries.resize(scene->mNumMeshes);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0; i < entries.size(); ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			initMesh(i, mesh, scene, fileName, bonesByName);
		}
		return true;
	}
	else {
		printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());
		return false;
	}
}

unsigned int Mesh::getNumSubMeshes() {
	return entries.size();
}

vector<glm::vec3> Mesh::getVertexPositions(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].vertexPositions;
	else {
		printf("This mesh has no entry %u\n", entry);
		return vector<glm::vec3>();
	}
}

vector<glm::vec3> Mesh::getVertexNormals(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].vertexNormals;
	else {
		printf("This mesh has no entry %u\n", entry);
		return vector<glm::vec3>();
	}
}

vector<glm::vec2> Mesh::getVertexTexCoords(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].vertexTexCoords;
	else {
		printf("This mesh has no entry %u\n", entry);
		return vector<glm::vec2>();
	}
}

vector<glm::vec4> Mesh::getVertexColors(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].vertexColors;
	else {
		printf("This mesh has no entry %u\n", entry);
		return vector<glm::vec4>();
	}
}

vector<unsigned int> Mesh::getVertexBoneIds(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].boneIds;
	else {
		printf("This mesh has no entry %u\n", entry);
		return vector<unsigned int>();
	}
}
vector<float> Mesh::getVertexBoneWeights(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].boneWeights;
	else {
		printf("This mesh has no entry %u\n", entry);
		return vector<float>();
	}
}

vector<unsigned int> Mesh::getIndices(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].indices;
	else {
		printf("This mesh has no entry %u\n", entry);
		return vector<unsigned int>();
	}
}

map<string, vector<VertexWeight>> Mesh::getWeightsByBoneName(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].weightsByBoneName;
	else {
		printf("This mesh has no entry %u\n", entry);
		return map<string, vector<VertexWeight>>();
	}
}

string Mesh::getTextureName(unsigned int entry) {
	if (entry < entries.size())
		return entries[entry].textureName;
	else {
		printf("This mesh has no entry %u\n", entry);
		return string();
	}
}

glm::vec3 Mesh::getAabbMin() {
	return aabbMin;
}

glm::vec3 Mesh::getAabbMax() {
	return aabbMax;
}

void Mesh::initMesh(unsigned int index, const aiMesh* mesh, const aiScene* scene, const std::string& fileName, map<string, Bone3D*>* bonesByName) {
	const aiVector3D zero(0.0f, 0.0f, 0.0f);
	const aiColor4D black(0.0f, 0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		const aiVector3D* pos = &(mesh->mVertices[i]);
		const aiVector3D* normal = mesh->HasNormals() ? &(mesh->mNormals[i]) : &zero;
		const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero;
		const aiColor4D* color = mesh->HasVertexColors(0) ? &(mesh->mColors[0][i]) : &black;

		if (i == 0) {
			aabbMin.x = aabbMax.x = pos->x;
			aabbMin.y = aabbMax.y = pos->y;
			aabbMin.z = aabbMax.z = pos->z;
		}
		else {
			aabbMin.x = std::min(aabbMin.x, pos->x);
			aabbMin.y = std::min(aabbMin.y, pos->y);
			aabbMin.z = std::min(aabbMin.z, pos->z);
			aabbMax.x = std::max(aabbMax.x, pos->x);
			aabbMax.y = std::max(aabbMax.y, pos->y);
			aabbMax.z = std::max(aabbMax.z, pos->z);
		}

		entries[index].vertexPositions.push_back(glm::vec3(pos->x, pos->y, pos->z));
		entries[index].vertexNormals.push_back(glm::vec3(normal->x, normal->y, normal->z));
		entries[index].vertexTexCoords.push_back(glm::vec2(texCoord->x, texCoord->y));
		entries[index].vertexColors.push_back(glm::vec4(color->r, color->g, color->b, color->a));

		if (bonesByName) {
			entries[index].boneIds.push_back(0);
			entries[index].boneIds.push_back(0);
			entries[index].boneIds.push_back(0);
			entries[index].boneIds.push_back(0);

			entries[index].boneWeights.push_back(0);
			entries[index].boneWeights.push_back(0);
			entries[index].boneWeights.push_back(0);
			entries[index].boneWeights.push_back(0);
		}
	}

	if (bonesByName) {
		for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
			string boneName(mesh->mBones[i]->mName.data);

			if (bonesByName->find(boneName) != bonesByName->end()) {
				for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j) {
					unsigned int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
					float weight = mesh->mBones[i]->mWeights[j].mWeight;
					for (unsigned int k = 0; k < 4; ++k) {
						if (entries[index].boneWeights[vertexId * 4 + k] == 0.0f) {
							entries[index].boneIds[vertexId * 4 + k] = (*bonesByName)[boneName]->boneId;
							entries[index].boneWeights[vertexId * 4 + k] = weight;

							VertexWeight w;
							w.vertexId = vertexId;
							w.weight = weight;
							entries[index].weightsByBoneName[boneName].push_back(w);
							break;
						}
					}
				}
			}
		}
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		const aiFace& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		entries[index].indices.push_back(face.mIndices[0]);
		entries[index].indices.push_back(face.mIndices[1]);
		entries[index].indices.push_back(face.mIndices[2]);
	}

	string::size_type slashIndex = fileName.find_last_of("/");
	string dir;

	if (slashIndex == string::npos) {
		dir = ".";
	}
	else if (slashIndex == 0) {
		dir = "/";
	}
	else {
		dir = fileName.substr(0, slashIndex);
	}

	const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	bool bHasTexture = false;
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
			string fullPath = dir + "/" + path.data;
			entries[index].textureName.assign(fullPath);
			//printf("%s\n", fullPath.c_str());
			bHasTexture = true;
		}
	}
	if (!bHasTexture) {
		entries[index].textureName.assign("Data/white.png");
		printf("Placeholder material loaded!\n");
	}
}

/*bool Mesh::initMaterials(const aiScene* scene, const std::string& fileName) {
	bool result = true;

	string::size_type slashIndex = fileName.find_last_of("/");
	string dir;

	if (slashIndex == string::npos) {
		dir = ".";
	}
	else if (slashIndex == 0) {
		dir = "/";
	}
	else {
		dir = fileName.substr(0, slashIndex);
	}

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
		const aiMaterial* material = scene->mMaterials[i];
		textures[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				string fullPath = dir + "/" + path.data;
				printf("%s\n", fullPath.c_str());
				textures[i] = new Texture();
				result = textures[i]->load(fullPath.c_str());
				if (!result) {
					delete textures[i];
					textures[i] = nullptr;
				}
			}
		}
		if (!textures[i]) {
			textures[i] = new Texture();
			result = textures[i]->load("Data/white.png");
			printf("Placeholder material loaded!\n");
			if (!result) {
				printf("Error loading mesh material!\n");
				break;
			}
		}
	}
	return result;
}

void Mesh::clear() {
	for (unsigned int i = 0; i < textures.size(); ++i) {
		delete textures[i];
	}
}*/
