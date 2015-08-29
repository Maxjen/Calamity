#pragma once

#include "../../Calamity/Action.h"
#include "../BoneSelection.h"

using namespace clm;

class SelectBones : public Action {
private:
	BoneSelection* selection;
	unordered_map<unsigned int, int> oldSelection;
	unsigned int oldMainSelectedBone;
	unordered_map<unsigned int, int> newSelection;
	unsigned int newMainSelectedBone;
	bool replaceCurrentSelection;
	bool absoluteSelection;
public:
	explicit SelectBones(BoneSelection* selection, unordered_map<unsigned int, int> newSelection, bool replaceCurrentSelection = false);
	void setAbsoluteSelection(bool absoluteSelection);
    void apply();
    void revert();
};
