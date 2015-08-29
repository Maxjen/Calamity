#include "MoveSelection3D.h"

MoveSelection3D::MoveSelection3D(Selection3D* selection) :
	meshObjectsToMove(),
	startingMeshObjectPositions(),
	deltaPosition()
{
	meshObjectsToMove = selection->getSelectedMeshObjects();
	for (auto& it : meshObjectsToMove)
		startingMeshObjectPositions.push_back(it->getPosition());
}

void MoveSelection3D::setDeltaPosition(glm::vec3 deltaPosition) {
	this->deltaPosition = deltaPosition;
}

list<glm::vec3> MoveSelection3D::getStartingMeshObjectPositions() {
	return startingMeshObjectPositions;
}

void MoveSelection3D::apply() {
	list<MeshObject*>::iterator meshIt = meshObjectsToMove.begin();
	list<glm::vec3>::iterator posIt = startingMeshObjectPositions.begin();
	for (; meshIt != meshObjectsToMove.end(); ++meshIt, ++posIt) {
		glm::vec3 newPosition = (*posIt) + deltaPosition;
		(*meshIt)->setPosition(newPosition.x, newPosition.y, newPosition.z);
	}
}

void MoveSelection3D::revert() {
	list<MeshObject*>::iterator meshIt = meshObjectsToMove.begin();
	list<glm::vec3>::iterator posIt = startingMeshObjectPositions.begin();
	for (; meshIt != meshObjectsToMove.end(); ++meshIt, ++posIt)
		(*meshIt)->setPosition(posIt->x, posIt->y, posIt->z);
}
