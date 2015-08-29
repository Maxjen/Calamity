#pragma once

#include "../../Calamity/Action.h"
#include "../LayerProperties.h"

using namespace clm;

class LayerProperties;

class ChangeLayerProperty : public Action {
private:
	LayerProperties* properties;
	unsigned int layerId;
	Property* property;
	wstring wOldValue;
	wstring wNewValue;
public:
	explicit ChangeLayerProperty(LayerProperties* properties, unsigned int layerId, Property* property,
						wstring wOldValue, wstring wNewValue);
    void apply();
    void revert();
};
