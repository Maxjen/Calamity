#pragma once

#include "../../Calamity/Action.h"
#include "../MapEditorSelection.h"

using namespace clm;

class RemoveObjects : public Action {
private:
	MapEditorSelection* selection;

	list<unsigned int> spritesToRemove;
	unsigned int oldMainSelectedSprite;
	unordered_map<unsigned int, unsigned int> verticesToDeselect;
	set<unsigned int> trianglesToRemove;
public:
	explicit RemoveObjects(MapEditorSelection* selection);
    void apply();
    void revert();
};
