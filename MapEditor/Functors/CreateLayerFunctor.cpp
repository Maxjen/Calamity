#include "CreateLayerFunctor.h"

CreateLayerFunctor::CreateLayerFunctor(ActionManager* actionManager, LayerManager* layerManager, ListView* layerList, unsigned int listViewFont, Functor* functor) {
	this->actionManager = actionManager;
	this->layerManager = layerManager;
	this->layerList = layerList;
	this->listViewFont = listViewFont;
	this->functor = functor;
}

void CreateLayerFunctor::operator()() {
	action = new CreateLayer(layerManager, layerList, listViewFont, L"new Layer", 1.0f, functor);
	actionManager->pushAction(action);
}
