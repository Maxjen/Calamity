#include "ScaleBonesHandler.h"

ScaleBonesHandler::ScaleBonesHandler(BoneSelection* selection, ActionManager* actionManager, float mouseX, float mouseY) :
	actionManager(actionManager),
	action(new ScaleBones(selection)),
	selection(selection),

	bonesToScale(),
	startingPositions(),

	mousePos(mouseX, mouseY),
	scaleCenter(),
	startingDist(0),
	scaleFactor(0)
{
	bonesToScale = selection->getSelectedBones();
	startingPositions = action->getStartingPositions();

	scaleCenter = action->getScaleCenter();
	startingDist = std::max(vectorMag(mousePos - scaleCenter), 1.0f);
}

bool ScaleBonesHandler::handleEvent(SDL_Event* event) {
	GameManager* gameManager = selection->getGameManager();
	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			mousePos.x += event->motion.xrel;
			mousePos.y -= event->motion.yrel;
			scaleFactor = vectorMag(mousePos - scaleCenter) / startingDist;

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
