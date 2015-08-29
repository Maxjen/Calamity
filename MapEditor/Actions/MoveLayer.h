#pragma once

#include "Action.h"
#include "../../Common/LayerManager.h"
#include "Gui/ListView.h"

using namespace clm;

class MoveLayer : public Action {
private:
	LayerManager* layerManager;
	ListView* layerList;
	unsigned int layerId;
	Functor* functor;
	bool bRaise;
public:
	explicit MoveLayer(LayerManager* layerManager, ListView* layerList, unsigned int layerId, Functor* functor, bool bRaise);
    void apply();
    void revert();
	//void reloadLayerList();
};
