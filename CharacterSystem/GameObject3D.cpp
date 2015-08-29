#include "GameObject3D.h"

glm::vec3 GameObject3D::getPosition() {
	return position;
}

void GameObject3D::setPosition(float posX, float posY, float posZ) {
	position = glm::vec3(posX, posY, posZ);
}

void GameObject3D::setScale(float scaleX, float scaleY, float scaleZ) {
	scale = glm::vec3(scaleX, scaleY, scaleZ);
}
