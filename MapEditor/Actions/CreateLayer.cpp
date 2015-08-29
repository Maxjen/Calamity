#include "CreateLayer.h"

CreateLayer::CreateLayer(LayerManager* layerManager, ListView* layerList, unsigned int listViewFont, wstring layerName, float scrollSpeed, Functor* functor) :
	layerManager(layerManager),
	layerList(layerList),
	listViewFont(listViewFont),
	layerId(-1),
	previousLayerId(),
	layerName(layerName),
	scrollSpeed(scrollSpeed),
	functor(functor),

	firstApply(true)
{
	previousLayerId = layerManager->getSelectedLayerId();
}

void CreateLayer::apply() {
    if (firstApply) {
		layerId = layerManager->addLayer(layerName, scrollSpeed);

        firstApply = false;
    }
	else
		layerManager->restoreLayer(layerId);

	//reloadLayerList();
	layerList->reload();
	layerManager->setSelectedLayerId(layerId);
	(*functor)();
}

void CreateLayer::revert() {
	layerManager->removeLayer(layerId);

	//reloadLayerList();
	layerList->reload();
	layerManager->setSelectedLayerId(previousLayerId);
	(*functor)();
}

/*void CreateLayer::reloadLayerList() {
	layerManager->setSelectedLayerId(-1);
	layerList->reload();
}*/
