#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "btBulletDynamicsCommon.h"
//#include "CharacterMeshObject.h"

class Camera {
private:
	void calculateValues();

	int screenWidth, screenHeight;

	glm::vec3 position;

	float horizontalAngle;
	float verticalAngle;

	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	btRigidBody* character;
public:
	explicit Camera(int screenWidth, int screenHeight);

	void setThirdPerson(btRigidBody* character);
	void update();

	glm::vec3 getPosition();
	glm::vec3 getDirectionVector();
	glm::vec3 getUpVector();
	glm::vec3 getRightVector();
	void setPosition(float x, float y, float z);
	void move(float forward, float sideways);
	void changeAngles(float horizontal, float vertical);
	glm::mat4 getViewMatrix();
};
