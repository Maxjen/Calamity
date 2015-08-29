#pragma once

#include "../../ActionHandler.h"
#include "../../Calamity/ActionManager.h"
#include "../Actions/ScaleObjects.h"

using namespace clm;

//class ScaleObjects;

class ScaleObjectsHandler : public ActionHandler {
private:
	MapEditorSelection* selection;
	ActionManager* actionManager;
	ScaleObjects* action;

	list<unsigned int> spritesToScale;
	list<Vec2> startingSpritePositions;
	list<float> startingSpriteScales;

	unordered_map<unsigned int, unsigned int> verticesToScale;
	list<Vec2> startingVertexPositions;

	Vec2 mousePos;
	Vec2 scaleCenter;
	float startingDist;
	float scaleFactor;
public:
	explicit ScaleObjectsHandler(MapEditorSelection* selection, ActionManager* actionManager, float mouseX, float mouseY);
	bool handleEvent(SDL_Event* event);
};
