#include "MoveSelection3DHandler.h"

MoveSelection3DHandler::MoveSelection3DHandler(Selection3D* selection, Camera* camera, ActionManager* actionManager) :
	selection(selection),
	camera(camera),
	actionManager(actionManager),
	action(new MoveSelection3D(selection)),

	meshObjectsToMove(),
	startingMeshObjectPositions(),

	deltaPosition(glm::vec3(0, 0, 0))
{
	meshObjectsToMove = selection->getSelectedMeshObjects();
	startingMeshObjectPositions = action->getStartingMeshObjectPositions();
}

bool MoveSelection3DHandler::handleEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_MOUSEMOTION:
		{
			deltaPosition += camera->getRightVector() * (((float)event->motion.xrel) / 100);
			deltaPosition -= camera->getUpVector() * (((float)event->motion.yrel) / 100);
			list<MeshObject*>::iterator meshIt = meshObjectsToMove.begin();
			list<glm::vec3>::iterator posIt = startingMeshObjectPositions.begin();
			for (; meshIt != meshObjectsToMove.end(); ++meshIt, ++posIt) {
				glm::vec3 newPosition = (*posIt) + deltaPosition;
				(*meshIt)->setPosition(newPosition.x, newPosition.y, newPosition.z);
			}

			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT) {
				action->setDeltaPosition(deltaPosition);
				actionManager->pushAction(action);
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}
