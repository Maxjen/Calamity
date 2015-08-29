#pragma once

#include <memory>
#include <set>

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include "GameObject3D.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"

using std::unique_ptr;
using std::set;

class ClothObject : public GameObject3D {
private:
	struct VertexPN {
		VertexPN(glm::vec3 pos, glm::vec3 normal) { this->pos = pos; this->normal = normal; }
		glm::vec3 pos;
		glm::vec3 normal;
	};

	//void loadCollisionObject(vector<glm::vec3> positions, vector<unsigned int> indices);
	void renderInternal();

	Camera* camera;
	MeshShader* meshShader;

	vector<VertexPN> verticesPN;
	vector<unsigned int> fixedVertices;
	vector<glm::vec3> fixedVertexOffsets;

	//unique_ptr<Mesh> mesh;
	GLuint vertexPNBufferId;
	GLuint vertexTBufferId;
	GLuint indexBufferId;
	unsigned int numIndices;
	GLuint vaoId;
	Texture texture;

	btSoftRigidDynamicsWorld* dynamicsWorld;

	unique_ptr<btSoftBody> softBody;

	bool bLoaded;
	bool bSelected;
public:
	explicit ClothObject(Camera* camera, MeshShader* meshShader, btSoftRigidDynamicsWorld* dynamicsWorld);
	~ClothObject();
	bool loadMesh(const string& fileName);
	void attachToRigidBody(btRigidBody* rigidBody);

	int getObjectType();
	void setPosition(float posX, float posY, float posZ);
	void setScale(float scaleX, float scaleY, float scaleZ);
	void toggleSelection();

	void update();
	void setAPosition(glm::vec3 position);
	void applyMatrix(glm::mat4 matrix);
	void render();
};
