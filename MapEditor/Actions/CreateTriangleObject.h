#pragma once

#include "../../Calamity/Action.h"
#include "../MapEditorSelection.h"

using namespace clm;

class CreateTriangleObject : public Action {
private:
	MapEditorSelection* selection;
	unsigned int triangleId;
	string textureName;

	list<unsigned int> oldSprites;
	unsigned int oldMainSelectedSprite;
	unordered_map<unsigned int, unsigned int> oldVertices;

	unsigned int layerId;
	unsigned int v1, v2, v3;

    bool firstApply;
public:
	explicit CreateTriangleObject(MapEditorSelection* selection, unsigned int layerId, const char* textureName);
	void setVertices(unsigned int v1, unsigned int v2, unsigned int v3);
    void apply();
    void revert();
};
