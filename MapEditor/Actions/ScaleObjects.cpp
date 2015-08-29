#include "ScaleObjects.h"

ScaleObjects::ScaleObjects(MapEditorSelection* selection, bool onlyMainSprite) :
	selection(selection),

	spritesToScale(),
	startingSpritePositions(),
	startingSpriteScales(),

	verticesToScale(),
	startingVertexPositions(),

	scaleCenter(),
	scaleFactor(),
	absoluteScale(false)
{
	GameManager* gameManager = selection->getGameManager();
	if (onlyMainSprite) {
		unsigned int mainSprite = selection->getMainSelectedSprite();
		spritesToScale.push_back(mainSprite);
		scaleCenter = ((SpriteObject*)gameManager->getObject(mainSprite))->getPosition();
		startingSpritePositions.push_back(scaleCenter);
		startingSpriteScales.push_back(((SpriteObject*)gameManager->getObject(mainSprite))->getScale());
	}
	else {
		scaleCenter = Vec2(0, 0);
		spritesToScale = selection->getSelectedSprites();
		for (auto& it : spritesToScale) {
			Vec2 cPos = ((SpriteObject*)gameManager->getObject(it))->getPosition();
			scaleCenter += cPos;
			startingSpritePositions.push_back(cPos);
			startingSpriteScales.push_back(((SpriteObject*)gameManager->getObject(it))->getScale());
		}

		SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
		verticesToScale = selection->getSelectedVertices();
		for (auto& it : verticesToScale) {
			Vec2 cPos = sharedTriangleData->getSharedVertexPosition(it.first);
			scaleCenter += cPos;
			startingVertexPositions.push_back(cPos);
		}

		scaleCenter /= spritesToScale.size() + verticesToScale.size();
	}
}

void ScaleObjects::setAbsoluteScale(bool absoluteScale) {
	this->absoluteScale = absoluteScale;
}

void ScaleObjects::setScaleFactor(float scaleFactor) {
	this->scaleFactor = scaleFactor;
}

Vec2 ScaleObjects::getScaleCenter() {
	return scaleCenter;
}

list<Vec2> ScaleObjects::getStartingSpritePositions() {
	return startingSpritePositions;
}

list<float> ScaleObjects::getStartingSpriteScales() {
	return startingSpriteScales;
}

list<Vec2> ScaleObjects::getStartingVertexPositions() {
	return startingVertexPositions;
}

void ScaleObjects::apply() {
	GameManager* gameManager = selection->getGameManager();
	list<unsigned int>::iterator sprite = spritesToScale.begin();
	list<Vec2>::iterator posIt = startingSpritePositions.begin();
	list<float>::iterator scaleIt = startingSpriteScales.begin();
	for (; sprite != spritesToScale.end(); ++sprite, ++posIt, ++scaleIt) {
		if (absoluteScale) {
			((SpriteObject*)gameManager->getObject(*sprite))->setScale(scaleFactor);
		}
		else {
			Vec2 positionFromCenter = (*posIt) - scaleCenter;
			positionFromCenter *= scaleFactor;
			Vec2 newPosition = scaleCenter + positionFromCenter;
			((SpriteObject*)gameManager->getObject(*sprite))->setPosition(newPosition.x, newPosition.y);
			((SpriteObject*)gameManager->getObject(*sprite))->setScale((*scaleIt) * scaleFactor);
		}
	}

	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
	unordered_map<unsigned int, unsigned int>::iterator vertex = verticesToScale.begin();
	posIt = startingVertexPositions.begin();
	for (; vertex != verticesToScale.end(); ++vertex, ++posIt) {
		Vec2 positionFromCenter = (*posIt) - scaleCenter;
		positionFromCenter *= scaleFactor;
		Vec2 newPosition = scaleCenter + positionFromCenter;
		sharedTriangleData->setSharedVertexPosition(vertex->first, newPosition.x, newPosition.y);
	}

	selection->updateSelection();
	selection->updateProperties();
}

void ScaleObjects::revert() {
	GameManager* gameManager = selection->getGameManager();
	list<unsigned int>::iterator sprite = spritesToScale.begin();
	list<Vec2>::iterator posIt = startingSpritePositions.begin();
	list<float>::iterator scaleIt = startingSpriteScales.begin();
	for (; sprite != spritesToScale.end(); ++sprite, ++posIt, ++scaleIt) {
		((SpriteObject*)gameManager->getObject(*sprite))->setPosition(posIt->x, posIt->y);
		((SpriteObject*)gameManager->getObject(*sprite))->setScale(*scaleIt);
	}

	SharedTriangleData* sharedTriangleData = selection->getSharedTriangleData();
	unordered_map<unsigned int, unsigned int>::iterator vertex = verticesToScale.begin();
	posIt = startingVertexPositions.begin();
	for (; vertex != verticesToScale.end(); ++vertex, ++posIt)
		sharedTriangleData->setSharedVertexPosition(vertex->first, posIt->x, posIt->y);

	selection->updateSelection();
	selection->updateProperties();
}
