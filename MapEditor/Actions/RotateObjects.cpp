#include "RotateObjects.h"

RotateObjects::RotateObjects(MapEditorSelection* selection, bool onlyMainSprite) :
	selection(selection),

	spritesToRotate(),
	startingSpritePositions(),
	startingSpriteRotations(),

	verticesToRotate(),
	startingVertexPositions(),

	rotationCenter(),
	angle(0),
	absoluteRotation(false)
{
	GameManager* gameManager = selection->getGameManager();
	if (onlyMainSprite) {
		unsigned int mainSprite = selection->getMainSelectedSprite();
		spritesToRotate.push_back(mainSprite);
		rotationCenter = ((SpriteObject*)gameManager->getObject(mainSprite))->getPosition();
		startingSpritePositions.push_back(rotationCenter);
		startingSpriteRotations.push_back(((SpriteObject*)gameManager->getObject(mainSprite))->getRotation());
	}
	else {
		rotationCenter = Vec2(0, 0);
		spritesToRotate = selection->getSelectedSprites();
		for (auto& it : spritesToRotate) {
			Vec2 cPos = ((SpriteObject*)gameManager->getObject(it))->getPosition();
			rotationCenter += cPos;
			startingSpritePositions.push_back(cPos);
			startingSpriteRotations.push_back(((SpriteObject*)gameManager->getObject(it))->getRotation());
		}

		SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
		verticesToRotate = selection->getSelectedVertices();
		for (auto& it : verticesToRotate) {
			Vec2 cPos = sharedTriangleData->getSharedVertexPosition(it.first);
			rotationCenter += cPos;
			startingVertexPositions.push_back(cPos);
		}

		rotationCenter /= spritesToRotate.size() + verticesToRotate.size();
	}
}

void RotateObjects::setAbsoluteRotation(bool absoluteRotation) {
	this->absoluteRotation = absoluteRotation;
}

void RotateObjects::setRotationAngle(float angle) {
	this->angle = angle;
}

Vec2 RotateObjects::getRotationCenter() {
	return rotationCenter;
}

list<Vec2> RotateObjects::getStartingSpritePositions() {
	return startingSpritePositions;
}

list<float> RotateObjects::getStartingSpriteRotations() {
	return startingSpriteRotations;
}

list<Vec2> RotateObjects::getStartingVertexPositions() {
	return startingVertexPositions;
}

void RotateObjects::apply() {
	GameManager* gameManager = selection->getGameManager();
	list<unsigned int>::iterator sprite = spritesToRotate.begin();
	list<Vec2>::iterator posIt = startingSpritePositions.begin();
	list<float>::iterator rotIt = startingSpriteRotations.begin();
	for (; sprite != spritesToRotate.end(); ++sprite, ++posIt, ++rotIt) {
		if (absoluteRotation) {
			((SpriteObject*)gameManager->getObject(*sprite))->setRotation(angle);
		}
		else {
			Vec2 newPosition = vectorRot2D((*posIt), rotationCenter, angle);
			((SpriteObject*)gameManager->getObject(*sprite))->setPosition(newPosition.x, newPosition.y);
			((SpriteObject*)gameManager->getObject(*sprite))->setRotation((*rotIt) + angle);
		}
	}

	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
	unordered_map<unsigned int, unsigned int>::iterator vertex = verticesToRotate.begin();
	posIt = startingVertexPositions.begin();
	for (; vertex != verticesToRotate.end(); ++vertex, ++posIt) {
		Vec2 newPosition = vectorRot2D((*posIt), rotationCenter, angle);
		sharedTriangleData->setSharedVertexPosition(vertex->first, newPosition.x, newPosition.y);
	}

	selection->updateSelection();
	selection->updateProperties();
}

void RotateObjects::revert() {
	GameManager* gameManager = selection->getGameManager();
	list<unsigned int>::iterator sprite = spritesToRotate.begin();
	list<Vec2>::iterator posIt = startingSpritePositions.begin();
	list<float>::iterator rotIt = startingSpriteRotations.begin();
	for (; sprite != spritesToRotate.end(); ++sprite, ++posIt, ++rotIt) {
		((SpriteObject*)gameManager->getObject(*sprite))->setPosition(posIt->x, posIt->y);
		((SpriteObject*)gameManager->getObject(*sprite))->setRotation(*rotIt);
	}

	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
	unordered_map<unsigned int, unsigned int>::iterator vertex = verticesToRotate.begin();
	posIt = startingVertexPositions.begin();
	for (; vertex != verticesToRotate.end(); ++vertex, ++posIt)
		sharedTriangleData->setSharedVertexPosition(vertex->first, posIt->x, posIt->y);

	selection->updateSelection();
	selection->updateProperties();
}
