#include "MoveBonesHandler.h"

MoveBonesHandler::MoveBonesHandler(BoneSelection* selection, ActionManager* actionManager) :
	actionManager(actionManager),
	action(new MoveBones(selection)),
	selection(selection),
	bonesToMove(),
	startingPositions(),
	deltaPosition(0, 0)
{
	bonesToMove = selection->getSelectedBones();
	startingPositions = action->getStartingPositions();
}

bool MoveBonesHandler::handleEvent(SDL_Event* event) {
	GameManager* gameManager = selection->getGameManager();
	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			deltaPosition.x += event->motion.xrel;
			deltaPosition.y -= event->motion.yrel;
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
