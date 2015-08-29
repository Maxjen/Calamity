#pragma once

#include <memory>

#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include "Mesh.h"
#include "Texture.h"
#include "../Calamity/Shader/MeshShader.h"
#include "CharacterEditor3DProperties.h"

using std::unique_ptr;

class CharacterEditor3DProperties;

class CharacterClothComponent {
private:
	struct ClothVertex {
		glm::vec3 position;
		unsigned int boneIds[4];
		float boneWeights[4];
		float maxDistance;
	};

	float maxDistanceScale;
	vector<ClothVertex> clothVertices;
	vector<unsigned int> fixedVertices;

	GLuint vertexPNBufferId;
	GLuint vertexTBufferId;
	GLuint indexBufferId;
	unsigned int numIndices;
	GLuint vaoId;
	Texture texture;

	//map<string, Bone3D*>* bonesByName;
	vector<Bone3D*> bonesById;
	MeshShader* meshShader;

	btSoftRigidDynamicsWorld* dynamicsWorld;
	unique_ptr<btSoftBody> softBody;
public:
	explicit CharacterClothComponent();
	~CharacterClothComponent();
	bool init(const string& fileName, map<string, Bone3D*>* bonesByName, MeshShader* meshShader, CharacterEditor3DProperties* characterEditor3DProperties, btSoftRigidDynamicsWorld* dynamicsWorld);
	void setMaxDistanceScale(float maxDistanceScale);

	void render();
	void update();
};
