#include "LoadLayerListFunctor.h"

LoadLayerListFunctor::LoadLayerListFunctor(ListView* layerList, LayerManager* layerManager, SpriteManager* spriteManager, unsigned int fontId, SelectLayerFunctor* functor) {
	this->layerList = layerList;
	this->layerManager = layerManager;
	this->spriteManager = spriteManager;
	this->fontId = fontId;
	this->functor = functor;
}

void LoadLayerListFunctor::operator()() {
	layerList->clear();
	list<unsigned int> layerIds = layerManager->getLayerIds();
	for (auto& it : layerIds) {
		wstring label = layerManager->getLayerName(it);
		LayerItem* item = new LayerItem(spriteManager, layerManager, label, it, fontId);
		item->setFunctor(functor);
		layerList->addItem(item);
	}
}
