#pragma once

#include "Action.h"
#include "../../Common/SpriteObject.h"
#include "../../Common/LayerManager.h"
#include "../MapEditorSelection.h"
#include "Gui/ListView.h"
#include "../../Common/Gui/LayerItem.h"

using namespace clm;

class RemoveLayer : public Action {
private:
	LayerManager* layerManager;
	MapEditorSelection* selection;
	ListView* layerList;
	unsigned int listViewFont;
	unsigned int layerId;
	unsigned int newLayerId;

	list<unsigned int> spritesToDeselect;
	unordered_map<unsigned int, unsigned int> verticesToDeselect;

	Functor* functor;
	bool firstApply;
public:
	explicit RemoveLayer(LayerManager* layerManager, MapEditorSelection* selection, ListView* layerList, unsigned int listViewFont, unsigned int layerId, Functor* functor);
    void apply();
    void revert();
	//void reloadLayerList();
};
