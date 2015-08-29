#include "MoveLayerFunctor.h"

MoveLayerFunctor::MoveLayerFunctor(ActionManager* actionManager, LayerManager* layerManager, ListView* layerList, Functor* functor, bool bRaise) {
	this->actionManager = actionManager;
	this->layerManager = layerManager;
	this->layerList = layerList;
	this->functor = functor;
	this->bRaise = bRaise;
}

void MoveLayerFunctor::operator()() {
	unsigned int layerId = layerManager->getSelectedLayerId();
	if (layerId != (unsigned int)-1) {
		action = new MoveLayer(layerManager, layerList, layerId, functor, bRaise);
		actionManager->pushAction(action);
	}
}
