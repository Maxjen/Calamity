#include "MoveObjects.h"

MoveObjects::MoveObjects(MapEditorSelection* selection, bool onlyMainSprite) :
	selection(selection),
	spritesToMove(),
	startingSpritePositions(),
	verticesToMove(),
	startingVertexPositions(),
	deltaPosition()
{
	GameManager* gameManager = selection->getGameManager();
	if (onlyMainSprite) {
		unsigned int mainSprite = selection->getMainSelectedSprite();
		spritesToMove.push_back(mainSprite);
		startingSpritePositions.push_back(((SpriteObject*)gameManager->getObject(mainSprite))->getPosition());
	}
	else {
		spritesToMove = selection->getSelectedSprites();
		for (auto& it : spritesToMove)
			startingSpritePositions.push_back(((SpriteObject*)gameManager->getObject(it))->getPosition());

		SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
		verticesToMove = selection->getSelectedVertices();
		for (auto& it : verticesToMove)
			startingVertexPositions.push_back(sharedTriangleData->getSharedVertexPosition(it.first));
	}
}

void MoveObjects::setDeltaPosition(Vec2 deltaPosition) {
	this->deltaPosition = deltaPosition;
}

list<Vec2> MoveObjects::getStartingSpritePositions() {
	return startingSpritePositions;
}

list<Vec2> MoveObjects::getStartingVertexPositions() {
	return startingVertexPositions;
}

void MoveObjects::apply() {
	GameManager* gameManager = selection->getGameManager();
	list<unsigned int>::iterator sprite = spritesToMove.begin();
	list<Vec2>::iterator posIt = startingSpritePositions.begin();
	for (; sprite != spritesToMove.end(); ++sprite, ++posIt) {
		Vec2 newPosition = (*posIt) + deltaPosition;
		((SpriteObject*)gameManager->getObject(*sprite))->setPosition(newPosition.x, newPosition.y);
	}

	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
	unordered_map<unsigned int, unsigned int>::iterator vertex = verticesToMove.begin();
	posIt = startingVertexPositions.begin();
	for (; vertex != verticesToMove.end(); ++vertex, ++posIt) {
		Vec2 newPosition = (*posIt) + deltaPosition;
		sharedTriangleData->setSharedVertexPosition(vertex->first, newPosition.x, newPosition.y);
	}

	selection->updateSelection();
	selection->updateProperties();
}

void MoveObjects::revert() {
	GameManager* gameManager = selection->getGameManager();
	list<unsigned int>::iterator sprite = spritesToMove.begin();
	list<Vec2>::iterator posIt = startingSpritePositions.begin();
	for (; sprite != spritesToMove.end(); ++sprite, ++posIt)
		((SpriteObject*)gameManager->getObject(*sprite))->setPosition(posIt->x, posIt->y);

	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
	unordered_map<unsigned int, unsigned int>::iterator vertex = verticesToMove.begin();
	posIt = startingVertexPositions.begin();
	for (; vertex != verticesToMove.end(); ++vertex, ++posIt)
		sharedTriangleData->setSharedVertexPosition(vertex->first, posIt->x, posIt->y);

	selection->updateSelection();
	selection->updateProperties();
}
