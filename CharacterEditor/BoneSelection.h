#pragma once

#include <map>

#include "../Common/GameManager.h"
#include "../Common/BoneObject.h"
#include "../Common/BoneManager.h"
#include "BoneProperties.h"

using std::map;

using namespace clm;

class BoneProperties;

class BoneSelection {
private:
	GameManager* gameManager;
	BoneManager* boneManager;

	// map to 0 (none), 1 (root), 2 (tip), 3 (both)
	unordered_map<unsigned int, int> selectedBones;
	unsigned int mainSelectedBone;

	BoneProperties* boneProperties;
public:
	BoneSelection(GameManager* gameManager, BoneManager* boneManager);
	void setBoneProperties(BoneProperties* boneProperties);
	void updateProperties();

	void selectBone(unsigned int boneId, int boneVertices);
	void selectBoneAbsolute(unsigned int boneId, int boneVertices);
	void setMainSelectedBone(unsigned int boneId);
	unordered_map<unsigned int, int> getSelectedBones();
	unsigned int getMainSelectedBone();
	GameManager* getGameManager();
	bool moreThanOneVertex();
};
