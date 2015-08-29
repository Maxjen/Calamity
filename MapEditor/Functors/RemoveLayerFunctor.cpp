#include "RemoveLayerFunctor.h"

RemoveLayerFunctor::RemoveLayerFunctor(ActionManager* actionManager, LayerManager* layerManager, MapEditorSelection* selection, ListView* layerList, unsigned int listViewFont, Functor* functor) {
	this->actionManager = actionManager;
	this->layerManager = layerManager;
	this->selection = selection;
	this->layerList = layerList;
	this->listViewFont = listViewFont;
	this->functor = functor;
}

void RemoveLayerFunctor::operator()() {
	unsigned int layerId = layerManager->getSelectedLayerId();
	if (layerId != (unsigned int)-1) {
		action = new RemoveLayer(layerManager, selection, layerList, listViewFont, layerId, functor);
		actionManager->pushAction(action);
	}
}
