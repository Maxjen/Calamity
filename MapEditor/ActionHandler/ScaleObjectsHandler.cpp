#include "ScaleObjectsHandler.h"

ScaleObjectsHandler::ScaleObjectsHandler(MapEditorSelection* selection, ActionManager* actionManager, float mouseX, float mouseY) :
	selection(selection),
	actionManager(actionManager),
	action(new ScaleObjects(selection)),

	spritesToScale(),
	startingSpritePositions(),
	startingSpriteScales(),

	verticesToScale(),
	startingVertexPositions(),

	mousePos(),
	scaleCenter(),
	startingDist(0),
	scaleFactor(1.0f)
{
	spritesToScale = selection->getSelectedSprites();
	startingSpritePositions = action->getStartingSpritePositions();
	startingSpriteScales = action->getStartingSpriteScales();

	verticesToScale = selection->getSelectedVertices();
	startingVertexPositions = action->getStartingVertexPositions();

	mousePos = Vec2(mouseX, mouseY);
	scaleCenter = action->getScaleCenter();
	startingDist = std::max(vectorMag(mousePos - scaleCenter), 1.0f);
}

bool ScaleObjectsHandler::handleEvent(SDL_Event* event) {
	GameManager* gameManager = selection->getGameManager();
	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			mousePos.x += event->motion.xrel;
			mousePos.y -= event->motion.yrel;
			scaleFactor = vectorMag(mousePos - scaleCenter) / startingDist;

			list<unsigned int>::iterator sprite = spritesToScale.begin();
			list<Vec2>::iterator posIt = startingSpritePositions.begin();
			list<float>::iterator scaleIt = startingSpriteScales.begin();
			for (; sprite != spritesToScale.end(); ++sprite, ++posIt, ++scaleIt) {
				Vec2 positionFromCenter = (*posIt) - scaleCenter;
				positionFromCenter *= scaleFactor;
				Vec2 newPosition = scaleCenter + positionFromCenter;
				((SpriteObject*)gameManager->getObject(*sprite))->setPosition(newPosition.x, newPosition.y);
				((SpriteObject*)gameManager->getObject(*sprite))->setScale((*scaleIt) * scaleFactor);
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
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				action->setScaleFactor(scaleFactor);
				actionManager->pushAction(action);
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
