#pragma once

#include "Action.h"
#include "../../Common/SpriteObject.h"
#include "../../Common/LayerManager.h"
#include "../Gui/ListView.h"

using namespace clm;

class CreateLayer : public Action {
private:
	LayerManager* layerManager;
	ListView* layerList;
	unsigned int listViewFont;
	unsigned int layerId;
	unsigned int previousLayerId;
	wstring layerName;
	float scrollSpeed;
	Functor* functor;

    bool firstApply;
public:
	explicit CreateLayer(LayerManager* layerManager, ListView* layerList, unsigned int listViewFont, wstring layerName, float scrollSpeed, Functor* functor);
    void apply();
    void revert();
	void reloadLayerList();
};
