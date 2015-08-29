#pragma once

#include "../../Calamity/Action.h"
#include "../MapEditorSelection.h"

using namespace clm;

class SelectObjects : public Action {
private:
	MapEditorSelection* selection;

	list<unsigned int> oldSprites;
	unsigned int oldMainSelectedSprite;
	unordered_map<unsigned int, unsigned int> oldVertices;

	list<unsigned int> newSprites;
	unsigned int newMainSelectedSprite;
	unordered_map<unsigned int, unsigned int> newVertices;
public:
	explicit SelectObjects(MapEditorSelection* selection, bool replaceCurrentSelection = false);
	void setSpritesToSelect(list<unsigned int> newSprites);
	void setVerticesToSelect(unordered_map<unsigned int, unsigned int> newVertices);
    void apply();
    void revert();
};
