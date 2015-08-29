#include "RotateObjectsHandler.h"

RotateObjectsHandler::RotateObjectsHandler(MapEditorSelection* selection, ActionManager* actionManager, float mouseX, float mouseY) :
	selection(selection),
	actionManager(actionManager),
	action(new RotateObjects(selection)),

	spritesToRotate(),
	startingSpritePositions(),
	startingSpriteRotations(),

	verticesToRotate(),
	startingVertexPositions(),

	mousePos(Vec2(mouseX, mouseY)),
	rotationCenter(),
	startingAngle(0),
	angle(0)
{
	spritesToRotate = selection->getSelectedSprites();
	startingSpritePositions = action->getStartingSpritePositions();
	startingSpriteRotations = action->getStartingSpriteRotations();

	verticesToRotate = selection->getSelectedVertices();
	startingVertexPositions = action->getStartingVertexPositions();

	rotationCenter = action->getRotationCenter();
	startingAngle = atan2(mousePos.y - rotationCenter.y, mousePos.x - rotationCenter.x);
}

bool RotateObjectsHandler::handleEvent(SDL_Event* event) {
	GameManager* gameManager = selection->getGameManager();
	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			mousePos.x += event->motion.xrel;
			mousePos.y -= event->motion.yrel;
			angle = atan2(mousePos.y - rotationCenter.y, mousePos.x - rotationCenter.x) - startingAngle;

			list<unsigned int>::iterator sprite = spritesToRotate.begin();
			list<Vec2>::iterator posIt = startingSpritePositions.begin();
			list<float>::iterator rotIt = startingSpriteRotations.begin();
			for (; sprite != spritesToRotate.end(); ++sprite, ++posIt, ++rotIt) {
				Vec2 newPosition = vectorRot2D((*posIt), rotationCenter, angle);
				((SpriteObject*)gameManager->getObject(*sprite))->setPosition(newPosition.x, newPosition.y);
				((SpriteObject*)gameManager->getObject(*sprite))->setRotation((*rotIt) + angle);
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
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				action->setRotationAngle(angle);
				actionManager->pushAction(action);
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
