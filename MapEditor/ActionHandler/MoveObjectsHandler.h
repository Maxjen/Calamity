#pragma once

#include "../../ActionHandler.h"
#include "../../Calamity/ActionManager.h"
#include "../Actions/MoveObjects.h"

using namespace clm;

//class MoveObjects;
//class MapEditorSelection;

class MoveObjectsHandler : public ActionHandler {
private:
	MapEditorSelection* selection;
	ActionManager* actionManager;
	MoveObjects* action;

	list<unsigned int> spritesToMove;
	list<Vec2> startingSpritePositions;

	unordered_map<unsigned int, unsigned int> verticesToMove;
	list<Vec2> startingVertexPositions;

	Vec2 deltaPosition;
public:
	explicit MoveObjectsHandler(MapEditorSelection* selection, ActionManager* actionManager);
	bool handleEvent(SDL_Event* event);
};
