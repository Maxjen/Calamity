#pragma once

#include "../../ActionHandler.h"
#include "../../Calamity/ActionManager.h"
#include "../Actions/CreateBones.h"

using namespace clm;

class CreateBonesHandler : public ActionHandler {
private:
	ActionManager* actionManager;
	CreateBones* action;
	BoneSelection* selection;
	list<Vec2> startingPositions;
	Vec2 boneVector;
	list<BoneObject*> tmpBones;
public:
	explicit CreateBonesHandler(BoneManager* boneManager, BoneSelection* selection, LoadBoneListFunctor* loadBoneListFunctor,
					   ActionManager* actionManager, SpriteManager* spriteManager, list<Vec2> startingPositions, unsigned int layerId);
	bool handleEvent(SDL_Event* event);
};
