#pragma once

#include "Functor.h"
#include "ActionManager.h"
#include "../../Common/LayerManager.h"
#include "../Actions/MoveLayer.h"

using namespace clm;

class MoveLayerFunctor : public Functor {
private:
	ActionManager* actionManager;
	LayerManager* layerManager;
	MoveLayer* action;
	ListView* layerList;
	Functor* functor;
	bool bRaise;
public:
	MoveLayerFunctor(ActionManager* actionManager, LayerManager* layerManager, ListView* layerList, Functor* functor, bool bRaise);
	void operator()();
};
