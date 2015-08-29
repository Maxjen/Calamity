#include "MoveBones.h"

MoveBones::MoveBones(BoneSelection* selection) :
	selection(selection),
	bonesToMove(),
	startingPositions(),
	deltaPosition()
{
	bonesToMove = selection->getSelectedBones();
	GameManager* gameManager = selection->getGameManager();
	for (auto& it : bonesToMove) {
		if (it.second == 1 || it.second == 3)
			startingPositions.push_back(((BoneObject*)gameManager->getObject(it.first))->getRootPosition());
		if (it.second == 2 || it.second == 3)
			startingPositions.push_back(((BoneObject*)gameManager->getObject(it.first))->getTipPosition());
	}
}

void MoveBones::setDeltaPosition(Vec2 deltaPosition) {
	this->deltaPosition = deltaPosition;
}

list<Vec2> MoveBones::getStartingPositions() {
	return startingPositions;
}

void MoveBones::apply() {
	GameManager* gameManager = selection->getGameManager();
	list<Vec2>::iterator posIt = startingPositions.begin();
	for (auto& it : bonesToMove) {
		if (it.second == 1 || it.second == 3) {
			Vec2 newPosition = *posIt + deltaPosition;
			((BoneObject*)gameManager->getObject(it.first))->setRootPosition(newPosition.x, newPosition.y);
			++posIt;
		}
		if (it.second == 2 || it.second == 3) {
			Vec2 newPosition = *posIt + deltaPosition;
			((BoneObject*)gameManager->getObject(it.first))->setTipPosition(newPosition.x, newPosition.y);
			++posIt;
		}
		gameManager->updateObjectAABB(it.first);
	}
}

void MoveBones::revert() {
	GameManager* gameManager = selection->getGameManager();
	list<Vec2>::iterator posIt = startingPositions.begin();
	for (auto& it : bonesToMove) {
		if (it.second == 1 || it.second == 3) {
			Vec2 newPosition = *posIt;
			((BoneObject*)gameManager->getObject(it.first))->setRootPosition(newPosition.x, newPosition.y);
			++posIt;
		}
		if (it.second == 2 || it.second == 3) {
			Vec2 newPosition = *posIt;
			((BoneObject*)gameManager->getObject(it.first))->setTipPosition(newPosition.x, newPosition.y);
			++posIt;
		}
		gameManager->updateObjectAABB(it.first);
	}
}
