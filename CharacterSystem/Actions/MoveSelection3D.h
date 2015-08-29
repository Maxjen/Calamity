#pragma once

#include "Action.h"
#include "../Selection3D.h"

using namespace clm;

class MoveSelection3D : public Action {
private:
	list<MeshObject*> meshObjectsToMove;
	list<glm::vec3> startingMeshObjectPositions;

	glm::vec3 deltaPosition;
public:
	explicit MoveSelection3D(Selection3D* selection);
	void setDeltaPosition(glm::vec3 deltaPosition);
	list<glm::vec3> getStartingMeshObjectPositions();
    void apply();
    void revert();
};
