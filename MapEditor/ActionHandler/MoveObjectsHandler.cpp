#include "MoveObjectsHandler.h"

MoveObjectsHandler::MoveObjectsHandler(MapEditorSelection* selection, ActionManager* actionManager) :
	selection(selection),
	actionManager(actionManager),
	action(new MoveObjects(selection)),

	spritesToMove(),
	startingSpritePositions(),

	verticesToMove(),
	startingVertexPositions(),

	deltaPosition(Vec2(0, 0))
{
	spritesToMove = selection->getSelectedSprites();
	startingSpritePositions = action->getStartingSpritePositions();

	verticesToMove = selection->getSelectedVertices();
	startingVertexPositions = action->getStartingVertexPositions();
}

bool MoveObjectsHandler::handleEvent(SDL_Event* event) {
	GameManager* gameManager = selection->getGameManager();
	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			deltaPosition.x += event->motion.xrel;
			deltaPosition.y -= event->motion.yrel;
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

			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				action->setDeltaPosition(deltaPosition);
				actionManager->pushAction(action);
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
