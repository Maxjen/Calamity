#pragma once

#include "Functor.h"
#include "ActionManager.h"
#include "../../Common/LayerManager.h"
#include "../Actions/CreateLayer.h"

using namespace clm;

class CreateLayerFunctor : public Functor {
private:
	ActionManager* actionManager;
	LayerManager* layerManager;
	CreateLayer* action;
	ListView* layerList;
	unsigned int listViewFont;
	Functor* functor;
public:
	CreateLayerFunctor(ActionManager* actionManager, LayerManager* layerManager, ListView* layerList, unsigned int listViewFont, Functor* functor);
	void operator()();
};
