#pragma once

#include "../../ActionHandler.h"
#include "../../Calamity/ActionManager.h"
#include "../Actions/RotateObjects.h"

using namespace clm;

//class RotateObjects;

class RotateObjectsHandler : public ActionHandler {
private:
	MapEditorSelection* selection;
	ActionManager* actionManager;
	RotateObjects* action;

	list<unsigned int> spritesToRotate;
	list<Vec2> startingSpritePositions;
	list<float> startingSpriteRotations;

	unordered_map<unsigned int, unsigned int> verticesToRotate;
	list<Vec2> startingVertexPositions;

	Vec2 mousePos;
	Vec2 rotationCenter;
	float startingAngle;
	float angle;
public:
	explicit RotateObjectsHandler(MapEditorSelection* selection, ActionManager* actionManager, float mouseX, float mouseY);
	bool handleEvent(SDL_Event* event);
};
