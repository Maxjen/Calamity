#pragma once

#include "Properties.h"
#include "ActionManager.h"
#include "../../Common/LayerManager.h"
#include "../../Common/GameManager.h"
#include "Actions/ChangeLayerProperty.h"
#include "Gui/ListView.h"

using namespace clm;

class LayerProperties : public Properties {
private:
	ActionManager* actionManager;
	LayerManager* layerManager;
	ListView* layerList;
	GameManager* gameManager;

	unsigned int layerId;

	StringProperty layerName;
	FloatProperty scrollSpeed;
public:
	explicit LayerProperties(ActionManager* actionManager, LayerManager* layerManager, GameManager* gameManager);
	void setLayerList(ListView* layerList);
	ListView* getLayerList();
	void setLayerId(unsigned int layerId);
	void pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue);
	void applyChange();

	StringProperty* getLayerNameProperty();
	FloatProperty* getScrollSpeedProperty();
};
