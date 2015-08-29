#include "RotateBones.h"

RotateBones::RotateBones(BoneSelection* selection) :
	selection(selection),
	bonesToRotate(),
	startingPositions(),
	rotationCenter(0, 0),
	angle(0)
{
	bonesToRotate = selection->getSelectedBones();
	GameManager* gameManager = selection->getGameManager();
	for (auto& it : bonesToRotate) {
		if (it.second == 1 || it.second == 3) {
			Vec2 cPos = ((BoneObject*)gameManager->getObject(it.first))->getRootPosition();
			startingPositions.push_back(cPos);
			rotationCenter += cPos;
		}
		if (it.second == 2 || it.second == 3) {
			Vec2 cPos = ((BoneObject*)gameManager->getObject(it.first))->getTipPosition();
			startingPositions.push_back(cPos);
			rotationCenter += cPos;
		}
	}
	rotationCenter /= startingPositions.size();
}

void RotateBones::setRotationAngle(float angle) {
	this->angle = angle;
}

Vec2 RotateBones::getRotationCenter() {
	return rotationCenter;
}

list<Vec2> RotateBones::getStartingPositions() {
	return startingPositions;
}

void RotateBones::apply() {
	GameManager* gameManager = selection->getGameManager();
	list<Vec2>::iterator posIt = startingPositions.begin();
	for (auto& it : bonesToRotate) {
		if (it.second == 1 || it.second == 3) {
			Vec2 newPosition = vectorRot2D((*posIt), rotationCenter, angle);
			((BoneObject*)gameManager->getObject(it.first))->setRootPosition(newPosition.x, newPosition.y);
			++posIt;
		}
		if (it.second == 2 || it.second == 3) {
			Vec2 newPosition = vectorRot2D((*posIt), rotationCenter, angle);
			((BoneObject*)gameManager->getObject(it.first))->setTipPosition(newPosition.x, newPosition.y);
			++posIt;
		}
		gameManager->updateObjectAABB(it.first);
	}
}

void RotateBones::revert() {
	GameManager* gameManager = selection->getGameManager();
	list<Vec2>::iterator posIt = startingPositions.begin();
	for (auto& it : bonesToRotate) {
		if (it.second == 1 || it.second == 3) {
			((BoneObject*)gameManager->getObject(it.first))->setRootPosition(posIt->x, posIt->y);
			++posIt;
		}
		if (it.second == 2 || it.second == 3) {
			((BoneObject*)gameManager->getObject(it.first))->setTipPosition(posIt->x, posIt->y);
			++posIt;
		}
		gameManager->updateObjectAABB(it.first);
	}
}
