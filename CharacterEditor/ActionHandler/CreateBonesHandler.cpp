#include "CreateBonesHandler.h"

CreateBonesHandler::CreateBonesHandler(BoneManager* boneManager, BoneSelection* selection, LoadBoneListFunctor* loadBoneListFunctor, ActionManager* actionManager,
									   SpriteManager* spriteManager, list<Vec2> startingPositions, unsigned int layerId) :
	actionManager(actionManager),
	action(new CreateBones(spriteManager, boneManager, selection, loadBoneListFunctor, startingPositions, layerId)),
	selection(selection),
	startingPositions(startingPositions),
	boneVector(0, 0),
	tmpBones()
{
	GameManager* gameManager = selection->getGameManager();

	for (auto& it : selection->getSelectedBones()) {
		BoneObject* b = (BoneObject*)gameManager->getObject(it.first);
		if (it.second == 1 || it.second == 3)
			b->toggleRootSelection();
		if (it.second == 2 || it.second == 3)
			b->toggleTipSelection();
		b->updateBoneOverlayColor();
	}

	for (auto& it : startingPositions) {
		BoneObject* b = new BoneObject(spriteManager, it.x, it.y, it.x, it.y);
		gameManager->addGameObject(b, layerId);
		b->toggleTipSelection();
		b->updateBoneOverlayColor();
		tmpBones.push_back(b);
	}
}

bool CreateBonesHandler::handleEvent(SDL_Event* event) {
	GameManager* gameManager = selection->getGameManager();
	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			boneVector.x += event->motion.xrel;
			boneVector.y -= event->motion.yrel;
			list<BoneObject*>::iterator itB = tmpBones.begin();
			for (auto& it : startingPositions) {
				(*itB)->setTipPosition(it.x + boneVector.x, it.y + boneVector.y);
				gameManager->updateObjectAABB((*itB)->objectId);
				++itB;
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				for (auto& it : selection->getSelectedBones()) {
					BoneObject* b = (BoneObject*)gameManager->getObject(it.first);
					if (it.second == 1 || it.second == 3)
						b->toggleRootSelection();
					if (it.second == 2 || it.second == 3)
						b->toggleTipSelection();
				}
				for (auto& it : tmpBones) {
					gameManager->deleteGameObject(it->objectId);
				}
				action->setBoneVector(boneVector);
				actionManager->pushAction(action);
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
