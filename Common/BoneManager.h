#pragma once

#include <unordered_map>
#include <map>
#include <list>
#include <string>

#include "GameManager.h"

using std::unordered_map;
using std::map;
using std::list;
using std::string;

using namespace clm;

class BoneManager {
private:
	struct Bone {
		unsigned int zOrder;
		wstring boneName;
	};

	Renderer* renderer;
	GameManager* gameManager;

	// maps boneId to Bone object (also contains removed bones)
	unordered_map<unsigned int, Bone> bones;

	// maps zOrder to boneId
	map<unsigned int, unsigned int> usedBones;
	unsigned int cZOrder;

	unsigned int selectedBoneId;
public:
	explicit BoneManager(Renderer* renderer, GameManager* gameManager);
	~BoneManager();

	unsigned int getSelectedBoneId();
	void setSelectedBoneId(unsigned int selectedBoneId);
	void selectLowerBone();

	unsigned int addBone(wstring boneName, unsigned int boneId);
	void removeBone(unsigned int boneId);
	void restoreBone(unsigned int boneId);

	void moveBoneUp(unsigned int boneId);
	void moveBoneDown(unsigned int boneId);

	wstring getBoneName(unsigned int boneId);
	void setBoneName(unsigned int boneId, wstring boneName);

	list<unsigned int> getBoneIds();

	/*void prepareLayers(float viewPosX, float viewPosY);
	void drawLayers();
	void drawOverlays();
	void clearLayers();*/


	//unordered_map<unsigned int, GameLayer*>* getLayers();
	//list<unsigned int> getLayerIds();
	/*
	float getLayerScrollSpeed(unsigned int layerId);
	list<unsigned int> getObjectsAtPosition(unsigned int layerId, float posX, float posY);
	GameObject* getObject(unsigned int objectId);*/
};
