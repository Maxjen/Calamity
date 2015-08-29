#pragma once

#include "../../ActionHandler.h"
#include "ActionManager.h"
#include "../Actions/MoveSelection3D.h"

using namespace clm;

//class MoveObjects;
//class MapEditorSelection;

class MoveSelection3DHandler : public ActionHandler {
private:
	Selection3D* selection;
	Camera* camera;
	ActionManager* actionManager;
	MoveSelection3D* action;

	list<MeshObject*> meshObjectsToMove;
	list<glm::vec3> startingMeshObjectPositions;

	glm::vec3 deltaPosition;
public:
	explicit MoveSelection3DHandler(Selection3D* selection, Camera* camera, ActionManager* actionManager);
	bool handleEvent(SDL_Event* event);
};
