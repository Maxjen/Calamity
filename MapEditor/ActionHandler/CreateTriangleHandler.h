#pragma once

#include "../../ActionHandler.h"
#include "../../Calamity/ActionManager.h"
#include "../Actions/CreateTriangleObject.h"

using namespace clm;

//class MoveObjects;
//class MapEditorSelection;

class CreateTriangleHandler : public ActionHandler {
private:
	unsigned int getClosestVertex();

	MapEditorSelection* selection;
	ActionManager* actionManager;
	CreateTriangleObject* action;

	unsigned int layerId;

	Vec2 mousePos;
	unsigned int v1, v2, v3;
	unsigned int v1Tmp, v2Tmp, v3Tmp;

	int timesClicked;
public:
	explicit CreateTriangleHandler(MapEditorSelection* selection, ActionManager* actionManager, unsigned int layerId, const char* textureName, float mouseX, float mouseY);
	void render();
	bool handleEvent(SDL_Event* event);
};
