#pragma once

#include "../../ActionHandler.h"
#include "../../Calamity/ActionManager.h"
#include "../Actions/ScaleBones.h"

using namespace clm;

class ScaleBonesHandler : public ActionHandler {
private:
	ActionManager* actionManager;
	ScaleBones* action;
	BoneSelection* selection;

	unordered_map<unsigned int, int> bonesToScale;
	list<Vec2> startingPositions;

	Vec2 mousePos;
	Vec2 scaleCenter;
	float startingDist;
	float scaleFactor;
public:
	explicit ScaleBonesHandler(BoneSelection* selection, ActionManager* actionManager, float mouseX, float mouseY);
	bool handleEvent(SDL_Event* event);
};
