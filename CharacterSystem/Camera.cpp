#include "Camera.h"
#include <math.h>
#include <algorithm>

Camera::Camera(int screenWidth, int screenHeight) :
	screenWidth(screenWidth),
	screenHeight(screenHeight),

	position(glm::vec3(0, 0, 0)),

	horizontalAngle(M_PI),
	verticalAngle(0),

	direction(),
	right(),
	up(),

	character(nullptr)
{
	calculateValues();
}

void Camera::setThirdPerson(btRigidBody* character) {
	this->character = character;
}

void Camera::update() {
	if (character) {
		/*position = character->getPosition();
		position.y += 0.5f;*/

		btVector3 characterPos = character->getCenterOfMassPosition();
		position.x = characterPos.getX();
		position.y = characterPos.getY() + 0.5f;
		position.z = characterPos.getZ();

		position -= direction * 4.0f;
	}
}

glm::vec3 Camera::getPosition() {
	return position;
}

glm::vec3 Camera::getDirectionVector() {
	return direction;
}

glm::vec3 Camera::getUpVector() {
	return up;
}

glm::vec3 Camera::getRightVector() {
	return right;
}

void Camera::setPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
}

void Camera::calculateValues() {
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	right = glm::vec3(
		sin(horizontalAngle - M_PI_2),
		0,
		cos(horizontalAngle - M_PI_2)
	);

	up = glm::cross(right, direction);
}

void Camera::move(float forward, float sideways) {
	/*glm::vec3 forwardDir = glm::cross(up, right);
	position += forwardDir * forward + right * sideways;*/
	position += direction * forward + right * sideways;
}

void Camera::changeAngles(float horizontal, float vertical) {
	horizontalAngle += horizontal;
	int toSubtract = horizontalAngle / (2 * M_PI);
	horizontalAngle -= toSubtract * (2 * M_PI);
	verticalAngle += vertical;
	verticalAngle = std::max((float)-M_PI_2, verticalAngle);
	verticalAngle = std::min((float)M_PI_2, verticalAngle);
	calculateValues();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(
		position,
		position + direction,
		up
	);
}
