#pragma once

#include "../../Calamity/Action.h"
#include "../MapEditorSelection.h"

using namespace clm;

class FillTriangle : public Action {
private:
	MapEditorSelection* selection;
	unsigned int triangleId;
	string textureName;

	unordered_map<unsigned int, unsigned int> verticesToReselect;

    bool firstApply;
public:
	explicit FillTriangle(MapEditorSelection* selection, const char* textureName);
    void apply();
    void revert();
};
