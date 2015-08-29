#pragma once

#include "Functor.h"
#include "../../Common/LayerManager.h"
#include "Gui/ListView.h"
#include "../../Common/Gui/LayerItem.h"
#include "SelectLayerFunctor.h"

using namespace clm;

class LoadLayerListFunctor : public Functor {
private:
	ListView* layerList;
	LayerManager* layerManager;
	SpriteManager* spriteManager;
	unsigned int fontId;
	SelectLayerFunctor* functor;
public:
	LoadLayerListFunctor(ListView* layerList, LayerManager* layerManager, SpriteManager* spriteManager, unsigned int fontId, SelectLayerFunctor* functor);
	void operator()();
};
