#pragma once

#include <memory>

#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "btBulletDynamicsCommon.h"

#include "GameObject3D.h"
#include "Camera.h"

using std::unique_ptr;

class CharacterController {
private:
	btDiscreteDynamicsWorld* dynamicsWorld;

	unique_ptr<btCollisionShape> collisionShape;
	unique_ptr<btRigidBody> rigidBody;

	float rotY;
	// -1: left, 0: no x-movement, 1: right
	int walkingX;
	// -1: backward, 0: no z-movement, 1: forward
	int walkingZ;

	btVector3 lastVelocity;
public:
	explicit CharacterController(btDiscreteDynamicsWorld* dynamicsWorld, glm::vec3 position);
	~CharacterController();

	void setWalkingX(int walkingX);
	int getWalkingX();
	void setWalkingZ(int walkingZ);
	int getWalkingZ();

	btRigidBody* getControllerRigidBody();
	glm::vec3 getPosition();
	glm::vec3 getHorizontalVelocity();
	float getRotY();

	void update(Camera* camera);
};
