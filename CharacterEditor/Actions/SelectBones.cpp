#include "SelectBones.h"

SelectBones::SelectBones(BoneSelection* selection, unordered_map<unsigned int, int> newSelection, bool replaceCurrentSelection) :
	selection(selection),
	oldSelection(),
	oldMainSelectedBone(),
	newSelection(newSelection),
	newMainSelectedBone(),
	replaceCurrentSelection(replaceCurrentSelection),
	absoluteSelection(false)
{
	//if(replaceCurrentSelection)
	this->oldSelection = selection->getSelectedBones();
	oldMainSelectedBone = selection->getMainSelectedBone();
	newMainSelectedBone = newSelection.begin()->first;
}

void SelectBones::setAbsoluteSelection(bool absoluteSelection) {
	this->absoluteSelection = absoluteSelection;
}

void SelectBones::apply() {
	if (replaceCurrentSelection) {
		for (auto& it : oldSelection)
			selection->selectBone(it.first, it.second);
	}
	for (auto& it : newSelection) {
		if (absoluteSelection)
			selection->selectBoneAbsolute(it.first, it.second);
		else
			selection->selectBone(it.first, it.second);
	}
	selection->setMainSelectedBone(newMainSelectedBone);
	/*if (!oldSelection.empty()) {
		for (auto& it : oldSelection)
			selection->selectBone(it.first, it.second);
	}
	for (auto& it : newSelection) {
		if (absoluteSelection)
			selection->selectBoneAbsolute(it.first, it.second);
		else
			selection->selectBone(it.first, it.second);
	}
	selection->setMainSelectedBone(newMainSelectedBone);*/
}

void SelectBones::revert() {
	for (auto& it : newSelection)
		selection->selectBone(it.first, it.second);
	for (auto& it : oldSelection)
		selection->selectBoneAbsolute(it.first, it.second);
	/*
	for (auto& it : newSelection)
		selection->selectBone(it.first, it.second);
	if (!oldSelection.empty()) {
		for (auto& it : oldSelection)
			selection->selectBone(it.first, it.second);
	*/
	selection->setMainSelectedBone(oldMainSelectedBone);
}
