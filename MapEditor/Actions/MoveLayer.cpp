#include "MoveLayer.h"

MoveLayer::MoveLayer(LayerManager* layerManager, ListView* layerList, unsigned int layerId, Functor* functor, bool bRaise) :
	layerManager(layerManager),
	layerList(layerList),
	layerId(layerId),
	functor(functor),
	bRaise(bRaise)
{}

void MoveLayer::apply() {
	if (bRaise)
		layerManager->moveLayerUp(layerId);
	else
		layerManager->moveLayerDown(layerId);

	//reloadLayerList();
	layerList->reload();
	layerManager->setSelectedLayerId(layerId);
	(*functor)();
}

void MoveLayer::revert() {
	if (bRaise)
		layerManager->moveLayerDown(layerId);
	else
		layerManager->moveLayerUp(layerId);

	//reloadLayerList();
	layerList->reload();
	layerManager->setSelectedLayerId(layerId);
	(*functor)();
}

/*void MoveLayer::reloadLayerList() {
	layerManager->setSelectedLayerId(-1);
	layerList->reload();
}*/
