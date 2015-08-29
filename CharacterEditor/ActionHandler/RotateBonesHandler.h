#pragma once

#include "../../ActionHandler.h"
#include "../../Calamity/ActionManager.h"
#include "../Actions/RotateBones.h"

using namespace clm;

class RotateBonesHandler : public ActionHandler {
private:
	ActionManager* actionManager;
	RotateBones* action;
	BoneSelection* selection;

	unordered_map<unsigned int, int> bonesToRotate;
	list<Vec2> startingPositions;

	Vec2 mousePos;
	Vec2 rotationCenter;
	float startingAngle;
	float angle;
public:
	explicit RotateBonesHandler(BoneSelection* selection, ActionManager* actionManager, float mouseX, float mouseY);
	bool handleEvent(SDL_Event* event);
};
