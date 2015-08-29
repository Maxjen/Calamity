#pragma once

#include "Functor.h"
#include "ActionManager.h"
#include "../../Common/LayerManager.h"
#include "../Actions/RemoveLayer.h"

using namespace clm;

class RemoveLayerFunctor : public Functor {
private:
	ActionManager* actionManager;
	LayerManager* layerManager;
	MapEditorSelection* selection;
	RemoveLayer* action;
	ListView* layerList;
	unsigned int listViewFont;
	Functor* functor;
public:
	RemoveLayerFunctor(ActionManager* actionManager, LayerManager* layerManager, MapEditorSelection* selection, ListView* layerList, unsigned int listViewFont, Functor* functor);
	void operator()();
};
