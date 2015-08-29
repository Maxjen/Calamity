#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <GL/glew.h>

#include <glm/gtx/transform.hpp>

#include "btBulletDynamicsCommon.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "../Calamity/Shader/MeshShader.h"

using std::unique_ptr;
using std::string;
using std::vector;
using std::map;

using namespace clm;

class Bone3D {
public:
	explicit Bone3D();
	~Bone3D();

	void addRigidBody(btDiscreteDynamicsWorld* dynamicsWorld, float radius, float height, glm::vec3 offset);

	string boneName;
	unsigned int boneId;

	bool hasOffsetMatrix;
	glm::mat4 boneOffset;
	glm::mat4 finalTransformation;
	glm::mat4 nodeTransformation;		// only needed for initialization of ragdoll
	glm::mat4 globalTransformation;

	glm::vec3 scaling;
	glm::quat rotation;
	glm::vec3 translation;

	Bone3D* parent;
	vector<Bone3D*> children;

	bool bHasRigidBody;
	btDiscreteDynamicsWorld* dynamicsWorld;
	glm::vec3 rigidBodyOffset;
	unique_ptr<btCollisionShape> collisionShape;
	unique_ptr<btRigidBody> rigidBody;
};

class Animation {
private:
	struct ScalingKey {
		float time;
		glm::vec3 scale;
	};

	struct RotationKey {
		float time;
		glm::quat rotation;
	};

	struct PositionKey {
		float time;
		glm::vec3 position;
	};

	struct BoneAnimation {
		vector<ScalingKey> scalingKeys;
		vector<RotationKey> rotationKeys;
		vector<PositionKey> positionKeys;
	};

	float ticksPerSecond;
	float duration;
	map<string, BoneAnimation> boneAnimations;
public:
	explicit Animation();
	void init(aiAnimation* animation, map<string, Bone3D*>* bonesByName);

	float getTicksPerSecond();
	float getDuration();

	bool hasBoneAnimation(const string& boneName);

	glm::vec3 interpolateScaling(float animationTime, const string& boneName);
	glm::quat interpolateRotation(float animationTime, const string& boneName);
	glm::vec3 interpolatePosition(float animationTime, const string& boneName);
};

struct VertexWeight {
	unsigned int vertexId;
	float weight;
};

class Mesh {
private:
	struct MeshEntry {
		vector<glm::vec3> vertexPositions;
		vector<glm::vec3> vertexNormals;
		vector<glm::vec2> vertexTexCoords;
		vector<glm::vec4> vertexColors;

		// 4 per vertex
		vector<unsigned int> boneIds;
		vector<float> boneWeights;

		vector<unsigned int> indices;

		map<string, vector<VertexWeight>> weightsByBoneName;

		string textureName;
		//unsigned int materialIndex;
	};

	aiNode* getRootBone(aiNode* node);
	Bone3D* createBoneHierarchy(aiNode* node, Bone3D* parent);

	void initFromScene(const aiScene* scene, const string& fileName);
	void initMesh(unsigned int index, const aiMesh* mesh, const aiScene* scene, const std::string& fileName, map<string, Bone3D*>* bonesByName = nullptr);
	//bool initMaterials(const aiScene* scene, const std::string& fileName);
	//void clear();

	Bone3D* boneHierarchy;
	bool bDeleteBoneHierarchy;
	map<string, Bone3D*> bonesByName;

	map<string, Animation> animations;

	glm::vec3 aabbMin, aabbMax;

	vector<MeshEntry> entries;
	//vector<Texture*> textures;
public:
	explicit Mesh();
	~Mesh();

	bool loadSkeleton(const string& fileName);
	Bone3D* extractBoneHierarchy();
	map<string, Bone3D*> getBonesByName();
	map<string, Animation> getAnimations();

	bool loadMesh(const string& fileName, bool triangulate);
	bool loadMeshWithWeights(const string& fileName, map<string, Bone3D*>* bonesByName, bool triangulate);
	unsigned int getNumSubMeshes();
	vector<glm::vec3> getVertexPositions(unsigned int entry);
	vector<glm::vec3> getVertexNormals(unsigned int entry);
	vector<glm::vec2> getVertexTexCoords(unsigned int entry);
	vector<glm::vec4> getVertexColors(unsigned int entry);
	vector<unsigned int> getVertexBoneIds(unsigned int entry);
	vector<float> getVertexBoneWeights(unsigned int entry);
	vector<unsigned int> getIndices(unsigned int entry);
	map<string, vector<VertexWeight>> getWeightsByBoneName(unsigned int entry);
	string getTextureName(unsigned int entry);
	glm::vec3 getAabbMin();
	glm::vec3 getAabbMax();
};
