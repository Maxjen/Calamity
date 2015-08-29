#include "RotateBonesHandler.h"

RotateBonesHandler::RotateBonesHandler(BoneSelection* selection, ActionManager* actionManager, float mouseX, float mouseY) :
	actionManager(actionManager),
	action(new RotateBones(selection)),
	selection(selection),

	bonesToRotate(),
	startingPositions(),

	mousePos(mouseX, mouseY),
	rotationCenter(),
	startingAngle(),
	angle(0)
{
	bonesToRotate = selection->getSelectedBones();
	startingPositions = action->getStartingPositions();

	rotationCenter = action->getRotationCenter();
	startingAngle = atan2(mousePos.y - rotationCenter.y, mousePos.x - rotationCenter.x);
}

bool RotateBonesHandler::handleEvent(SDL_Event* event) {
	GameManager* gameManager = selection->getGameManager();
	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			mousePos.x += event->motion.xrel;
			mousePos.y -= event->motion.yrel;
			angle = atan2(mousePos.y - rotationCenter.y, mousePos.x - rotationCenter.x) - startingAngle;

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

			/*list<unsigned int>::iterator sprite = spritesToRotate.begin();
			list<Vec2>::iterator posIt = startingPositions.begin();
			list<float>::iterator rotIt = startingRotations.begin();
			for (; sprite != spritesToRotate.end(); ++sprite, ++posIt, ++rotIt) {
				Vec2 newPosition = vectorRot2D((*posIt), rotationCenter, angle);
				((SpriteObject*)gameManager->getObject(*sprite))->setPosition(newPosition.x, newPosition.y);
				((SpriteObject*)gameManager->getObject(*sprite))->setRotation((*rotIt) + angle);
				gameManager->updateObjectAABB(*sprite);
			}*/
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
