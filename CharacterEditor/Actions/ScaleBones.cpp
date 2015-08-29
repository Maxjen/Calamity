#include "ScaleBones.h"

ScaleBones::ScaleBones(BoneSelection* selection) :
	selection(selection),
	bonesToScale(),
	startingPositions(),
	scaleCenter(0, 0),
	scaleFactor(1.0f)
{
	bonesToScale = selection->getSelectedBones();
	GameManager* gameManager = selection->getGameManager();
	for (auto& it : bonesToScale) {
		if (it.second == 1 || it.second == 3) {
			Vec2 cPos = ((BoneObject*)gameManager->getObject(it.first))->getRootPosition();
			startingPositions.push_back(cPos);
			scaleCenter += cPos;
		}
		if (it.second == 2 || it.second == 3) {
			Vec2 cPos = ((BoneObject*)gameManager->getObject(it.first))->getTipPosition();
			startingPositions.push_back(cPos);
			scaleCenter += cPos;
		}
	}
	scaleCenter /= startingPositions.size();
}

void ScaleBones::setScaleFactor(float scaleFactor) {
	this->scaleFactor = scaleFactor;
}

Vec2 ScaleBones::getScaleCenter() {
	return scaleCenter;
}

list<Vec2> ScaleBones::getStartingPositions() {
	return startingPositions;
}

void ScaleBones::apply() {
	GameManager* gameManager = selection->getGameManager();
	list<Vec2>::iterator posIt = startingPositions.begin();
	for (auto& it : bonesToScale) {
		if (it.second == 1 || it.second == 3) {
			Vec2 positionFromCenter = (*posIt) - scaleCenter;
			positionFromCenter *= scaleFactor;
			Vec2 newPosition = scaleCenter + positionFromCenter;
			((BoneObject*)gameManager->getObject(it.first))->setRootPosition(newPosition.x, newPosition.y);
			++posIt;
		}
		if (it.second == 2 || it.second == 3) {
			Vec2 positionFromCenter = (*posIt) - scaleCenter;
			positionFromCenter *= scaleFactor;
			Vec2 newPosition = scaleCenter + positionFromCenter;
			((BoneObject*)gameManager->getObject(it.first))->setTipPosition(newPosition.x, newPosition.y);
			++posIt;
		}
		gameManager->updateObjectAABB(it.first);
	}
}

void ScaleBones::revert() {
	GameManager* gameManager = selection->getGameManager();
	list<Vec2>::iterator posIt = startingPositions.begin();
	for (auto& it : bonesToScale) {
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
