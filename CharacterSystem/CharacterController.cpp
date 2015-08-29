#include "CharacterController.h"
#include <math.h>
#include <algorithm>

CharacterController::CharacterController(btDiscreteDynamicsWorld* dynamicsWorld, glm::vec3 position) :
	dynamicsWorld(dynamicsWorld),

	collisionShape(),
	rigidBody(),

	rotY(0),
	walkingX(0),
	walkingZ(0),

	lastVelocity(0, 0, 0)
{
	collisionShape.reset(new btSphereShape(0.5f));
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(position.x, position.y, position.z)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape.get(), fallInertia);
	rigidBody.reset(new btRigidBody(constructionInfo));
	dynamicsWorld->addRigidBody(rigidBody.get(), COL_CONTROLLER, COL_WORLD);

	//rigidBody->setFriction(10.0f);
	/*printf("angDamping %f linDamping %f\n", rigidBody->getAngularDamping(), rigidBody->getLinearDamping());
	btRigidBody b;
	b.setDamping();*/


	/*btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin (btVector3(10.210098,-1.6433364,16.453260));
	ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	btScalar characterHeight = 1.75;
	btScalar characterWidth = 1.75;
	btConvexShape* capsule = new btCapsuleShape(characterWidth, characterHeight);
	ghostObject->setCollisionShape(capsule);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	btScalar stepHeight = btScalar(0.35);
	character = new btKinematicCharacterController (ghostObject, capsule, stepHeight);

	dynamicsWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	dynamicsWorld->addAction(character);*/


	/*collisionShape.reset(new btBoxShape(btVector3(1.0f, 1.0f, 1.0f)));
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, motionState, collisionShape.get(), btVector3(0, 0, 0));
	rigidBody.reset(new btRigidBody(constructionInfo));
	dynamicsWorld->addRigidBody(rigidBody.get());
	rigidBody->setUserPointer((void*)this);*/
}

CharacterController::~CharacterController() {
	dynamicsWorld->removeRigidBody(rigidBody.get());
	delete rigidBody->getMotionState();
}

void CharacterController::setWalkingX(int walkingX) {
	this->walkingX = walkingX;
}

int CharacterController::getWalkingX() {
	return walkingX;
}

void CharacterController::setWalkingZ(int walkingZ) {
	this->walkingZ = walkingZ;
}

int CharacterController::getWalkingZ() {
	return walkingZ;
}

btRigidBody* CharacterController::getControllerRigidBody() {
	return rigidBody.get();
}

glm::vec3 CharacterController::getPosition() {
	btVector3 pos = rigidBody->getCenterOfMassPosition();
	return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}

glm::vec3 CharacterController::getHorizontalVelocity() {
	btVector3 vel = rigidBody->getLinearVelocity();
	return glm::vec3(vel.getX(), 0, vel.getZ());
}

float CharacterController::getRotY() {
	return rotY;
}

void CharacterController::update(Camera* camera) {
	float accelerationForce = 20.0f;
	float maxSpeed = 10.0f;
	glm::vec3 direction(0, 0, 0);

	if (walkingX != 0) {
		glm::vec3 directionX = camera->getRightVector() * (float)walkingX;
		directionX.y = 0;
		directionX = glm::normalize(directionX);
		direction += directionX;
	}
	if (walkingZ != 0) {
		glm::vec3 directionZ = camera->getDirectionVector() * (float)walkingZ;
		directionZ.y = 0;
		directionZ = glm::normalize(directionZ);
		direction += directionZ;
	}
	if (direction != glm::vec3(0, 0, 0))
		direction = glm::normalize(direction);

	if (walkingX != 0 || walkingZ != 0) {
		direction *= accelerationForce;
		rigidBody->activate();
		rigidBody->applyCentralForce(btVector3(direction.x, 0, direction.z));
		//rigidBody->setLinearVelocity(btVector3(direction.x, 0, direction.z));
		btVector3 vel = rigidBody->getLinearVelocity();
		if (vel.length() > maxSpeed) {
			vel.normalize();
			vel *= maxSpeed;
			rigidBody->setLinearVelocity(vel);
		}
		lastVelocity = vel;
	}
	else {
		btVector3 vel = rigidBody->getLinearVelocity();
		if (vel.dot(lastVelocity) < 0 || vel.length() <= 0.1f) {
			vel.setX(0);
			vel.setZ(0);
			rigidBody->setLinearVelocity(vel);
			rigidBody->setAngularVelocity(btVector3(0, 0, 0));
		}
		else if ((lastVelocity.getX() != 0 || lastVelocity.getZ() != 0) && (vel.getX() != 0 || vel.getZ() != 0)) {
			glm::vec3 direction2(-vel.getX(), 0, -vel.getZ());
			direction2 = glm::normalize(direction2);
			direction2 *= accelerationForce;

			rigidBody->applyCentralForce(btVector3(direction2.x, 0, direction2.z));
		}

		/*btVector3 vel = rigidBody->getLinearVelocity();
		vel.setX(0);
		vel.setZ(0);
		rigidBody->setLinearVelocity(vel);
		rigidBody->setAngularVelocity(btVector3(0, 0, 0));*/

		lastVelocity = vel;
	}

	btVector3 vel = rigidBody->getLinearVelocity();
	if (vel.getX() != 0 && vel.getZ() != 0)
		rotY = atan2(vel.getX(), vel.getZ()) * 180 / M_PI;
}
