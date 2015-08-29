#pragma once

#include "../../ActionHandler.h"
#include "../../Calamity/ActionManager.h"
#include "../Actions/MoveBones.h"

using namespace clm;

class MoveBonesHandler : public ActionHandler {
private:
	ActionManager* actionManager;
	MoveBones* action;
	BoneSelection* selection;
	unordered_map<unsigned int, int> bonesToMove;
	list<Vec2> startingPositions;
	Vec2 deltaPosition;
public:
	explicit MoveBonesHandler(BoneSelection* selection, ActionManager* actionManager);
	bool handleEvent(SDL_Event* event);
};
