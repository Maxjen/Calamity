#pragma once

#include <memory>

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "GameObject3D.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"

using std::unique_ptr;

using namespace clm;

class MeshObject : public GameObject3D {
private:
	struct Vertex {
		explicit Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 texCoord) :
			pos(pos),
			normal(normal),
			texCoord(texCoord)
		{}

		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	void loadCollisionObject();
	void renderInternal();

	Camera* camera;
	MeshShader* meshShader;

	//unique_ptr<Mesh> mesh;
	GLuint vertexBufferId;
	GLuint indexBufferId;
	unsigned int numIndices;
	GLuint vaoId;
	Texture texture;

	glm::vec3 aabbMin, aabbMax;

	//btDiscreteDynamicsWorld* dynamicsWorld;
	btSoftRigidDynamicsWorld* dynamicsWorld;

	unique_ptr<btCollisionShape> collisionShape;
	//unique_ptr<btDefaultMotionState> motionState;
	unique_ptr<btRigidBody> rigidBody;

	bool bLoaded;
	bool bSelected;
public:
	explicit MeshObject(Camera* camera, MeshShader* meshShader, btSoftRigidDynamicsWorld* dynamicsWorld);
	~MeshObject();
	bool loadMesh(const string& fileName);
	btRigidBody* getRigidBody();

	int getObjectType();
	void setPosition(float posX, float posY, float posZ);
	void setScale(float scaleX, float scaleY, float scaleZ);
	void toggleSelection();

	void render();
};
