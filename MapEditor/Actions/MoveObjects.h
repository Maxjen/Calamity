#pragma once

#include "../../Calamity/Math/Vec2.h"
#include "../../Calamity/Action.h"
#include "../MapEditorSelection.h"

using namespace clm;

class MapEditorSelection;

class MoveObjects : public Action {
private:
	MapEditorSelection* selection;

	list<unsigned int> spritesToMove;
	list<Vec2> startingSpritePositions;

	unordered_map<unsigned int, unsigned int> verticesToMove;
	list<Vec2> startingVertexPositions;

	Vec2 deltaPosition;
public:
	explicit MoveObjects(MapEditorSelection* selection, bool onlyMainSprite = false);
	void setDeltaPosition(Vec2 deltaPosition);
	list<Vec2> getStartingSpritePositions();
	list<Vec2> getStartingVertexPositions();
    void apply();
    void revert();
};
