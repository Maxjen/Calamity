#include "CompositionManager.h"

CompositionManager::CompositionManager(Renderer* renderer, GameManager* gameManager) :
	renderer(renderer),
	gameManager(gameManager),

	layers(),
	usedLayers(),
	cZOrder(0),

	selectedLayerId(-1)
{}

CompositionManager::~CompositionManager() {
	/*for (auto& it : layers)
		delete it.second;
	for (auto& it : objects)
		delete it.second;*/
}

unsigned int CompositionManager::getSelectedLayerId() {
	return selectedLayerId;
}

void CompositionManager::setSelectedLayerId(unsigned int selectedLayerId) {
	//if (layers.find(selectedLayerId) != layers.end() && usedLayers.find(layers[selectedLayerId].zOrder) != usedLayers.end())
	this->selectedLayerId = selectedLayerId;
}

void CompositionManager::selectLowerLayer() {
	if (selectedLayerId != (unsigned int)-1) {
		map<unsigned int, unsigned int>::iterator it = usedLayers.find(layers[selectedLayerId].zOrder);
		if (it != usedLayers.end() && it != usedLayers.begin()) {
			--it;
			selectedLayerId = it->second;
		}
		else
			selectedLayerId = -1;
	}
}

unsigned int CompositionManager::addLayer(wstring layerName, float scrollSpeed) {
	unsigned int layerId = gameManager->addGameLayer(scrollSpeed);

	Layer& layer = layers[layerId];
	layer.zOrder = cZOrder;
	layer.layerName = layerName;

	usedLayers[cZOrder] = layerId;

	++cZOrder;

	return layerId;
}

// TODO: actually remove objects from layers
void CompositionManager::removeLayer(unsigned int layerId) {
	if (layers.find(layerId) != layers.end() && usedLayers.find(layers[layerId].zOrder) != usedLayers.end()) {
		usedLayers.erase(layers[layerId].zOrder);
	}
	else
		printf("There is no layer with id %u\n", layerId);
}

void CompositionManager::restoreLayer(unsigned int layerId) {
	if (layers.find(layerId) != layers.end() && usedLayers.find(layers[layerId].zOrder) == usedLayers.end()) {
		usedLayers[layers[layerId].zOrder] = layerId;
	}
	else
		printf("Can't restore layer with id %u\n", layerId);
}

void CompositionManager::moveLayerUp(unsigned int layerId) {
	if (layers.find(layerId) != layers.end() && usedLayers.find(layers[layerId].zOrder) != usedLayers.end()) {
		map<unsigned int, unsigned int>::iterator cLayer = usedLayers.find(layers[layerId].zOrder);
		map<unsigned int, unsigned int>::iterator uLayer = cLayer;
		if (cLayer != --usedLayers.end()) {
			++uLayer;
			{
				// swapp zOrder in layers
				unsigned int zOrderTmp = cLayer->first;
				layers[cLayer->second].zOrder = uLayer->first;
				layers[uLayer->second].zOrder = zOrderTmp;

				// swapp values (layerId) in usedLayers
				unsigned int layerIdTmp = cLayer->second;
				cLayer->second = uLayer->second;
				uLayer->second = layerIdTmp;
			}
		}
	}
	else
		printf("There is no layer with id %u\n", layerId);
}

void CompositionManager::moveLayerDown(unsigned int layerId) {
	if (layers.find(layerId) != layers.end() && usedLayers.find(layers[layerId].zOrder) != usedLayers.end()) {
		map<unsigned int, unsigned int>::iterator cLayer = usedLayers.find(layers[layerId].zOrder);
		map<unsigned int, unsigned int>::iterator lLayer = cLayer;
		if (cLayer != usedLayers.begin()) {
			--lLayer;
			{
				// swapp zOrder in layers
				unsigned int zOrderTmp = cLayer->first;
				layers[cLayer->second].zOrder = lLayer->first;
				layers[lLayer->second].zOrder = zOrderTmp;

				// swapp values (layerId) in usedLayers
				unsigned int layerIdTmp = cLayer->second;
				cLayer->second = lLayer->second;
				lLayer->second = layerIdTmp;
			}
		}
	}
	else
		printf("There is no layer with id %u\n", layerId);
}

wstring CompositionManager::getLayerName(unsigned int layerId) {
	if (layers.find(layerId) != layers.end() && usedLayers.find(layers[layerId].zOrder) != usedLayers.end()) {
		return layers[layerId].layerName;
	}
	else {
		printf("There is no layer with id %u\n", layerId);
		return 0;
	}
}

void CompositionManager::setLayerName(unsigned int layerId, wstring layerName) {
	if (layers.find(layerId) != layers.end() && usedLayers.find(layers[layerId].zOrder) != usedLayers.end()) {
		layers[layerId].layerName = layerName;
	}
	else
		printf("There is no layer with id %u\n", layerId);
}

list<unsigned int> CompositionManager::getLayerIds() {
	list<unsigned int> result;
	for (auto& it : usedLayers)
		result.push_back(it.second);
	result.reverse();
	return result;
}

void CompositionManager::prepareLayers(float viewPosX, float viewPosY, float eyeOffset) {
	for (auto& it : usedLayers)
		gameManager->prepareLayer(it.second, viewPosX, viewPosY, eyeOffset);
}

void CompositionManager::drawLayers(float eyeOffset) {
	for (auto& it : usedLayers)
		gameManager->drawLayer(it.second, eyeOffset);
}

void CompositionManager::drawOverlays(float eyeOffset) {
	for (auto& it : usedLayers)
		gameManager->drawOverlay(it.second, eyeOffset);
}

void CompositionManager::clearLayers() {
	for (auto& it : usedLayers)
		gameManager->clearLayer(it.second);
}

void CompositionManager::drawLayer(unsigned int layerId) {
	gameManager->drawLayer(layerId, 0);
}

/*
list<unsigned int> GameManager::getLayerIds() {
	list<unsigned int> result;
	for (auto& it : layers)
		result.push_back(it.first);
	return result;
}

string GameManager::getLayerName(unsigned int layerId) {
	return layers[layerId]->getLayerName();
}

float GameManager::getLayerScrollSpeed(unsigned int layerId) {
	return layers[layerId]->getScrollSpeed();
}
*/
