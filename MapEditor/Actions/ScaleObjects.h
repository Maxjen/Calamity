#pragma once

#include "../../Calamity/Math/Vec2.h"
#include "../../Calamity/Action.h"
#include "../MapEditorSelection.h"

using namespace clm;

class MapEditorSelection;

class ScaleObjects : public Action {
private:
	MapEditorSelection* selection;

	list<unsigned int> spritesToScale;
	list<Vec2> startingSpritePositions;
	list<float> startingSpriteScales;

	unordered_map<unsigned int, unsigned int> verticesToScale;
	list<Vec2> startingVertexPositions;

	Vec2 scaleCenter;
	float scaleFactor;
	bool absoluteScale;
public:
	explicit ScaleObjects(MapEditorSelection* selection, bool onlyMainSprite = false);
	void setAbsoluteScale(bool absoluteScale);
	void setScaleFactor(float scaleFactor);
	Vec2 getScaleCenter();
	list<Vec2> getStartingSpritePositions();
	list<float> getStartingSpriteScales();
	list<Vec2> getStartingVertexPositions();
    void apply();
    void revert();
};
