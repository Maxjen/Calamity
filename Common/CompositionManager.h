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

class CompositionLayer {
private:
	vector<unsigned int> children;
public:
	void addChild(unsigned int childId);
	virtual void prepare() {};
	virtual void draw();
};

class CompositionManager {
private:
	struct CompositionLayer {
		unsigned int zOrder;
		wstring layerName;
	};

	Renderer* renderer;
	GameManager* gameManager;

	// maps layerId to Layer object (also contains removed layers)
	unordered_map<unsigned int, CompositionLayer> layers;

	// maps zOrder to layerId
	map<unsigned int, unsigned int> usedLayers;
	unsigned int cZOrder;

	unsigned int selectedLayerId;
public:
	explicit CompositionManager(Renderer* renderer, GameManager* gameManager);
	~CompositionManager();

	unsigned int getSelectedLayerId();
	void setSelectedLayerId(unsigned int selectedLayerId);
	void selectLowerLayer();

	unsigned int addLayer(wstring layerName, float scrollSpeed);
	void removeLayer(unsigned int layerId);
	void restoreLayer(unsigned int layerId);

	void moveLayerUp(unsigned int layerId);
	void moveLayerDown(unsigned int layerId);

	wstring getLayerName(unsigned int layerId);
	void setLayerName(unsigned int layerId, wstring layerName);

	list<unsigned int> getLayerIds();

	void prepareLayers(float viewPosX, float viewPosY, float eyeOffset = 0);
	void drawLayers(float eyeOffset = 0);
	void drawOverlays(float eyeOffset = 0);
	void clearLayers();

	void drawLayer(unsigned int layerId); // remove later


	//unordered_map<unsigned int, GameLayer*>* getLayers();
	//list<unsigned int> getLayerIds();
	/*
	float getLayerScrollSpeed(unsigned int layerId);
	list<unsigned int> getObjectsAtPosition(unsigned int layerId, float posX, float posY);
	GameObject* getObject(unsigned int objectId);*/
};
