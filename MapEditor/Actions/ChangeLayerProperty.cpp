#include "ChangeLayerProperty.h"

ChangeLayerProperty::ChangeLayerProperty(LayerProperties* properties, unsigned int layerId, Property* property,
										 wstring wOldValue, wstring wNewValue) :
	properties(properties),
	layerId(layerId),
	property(property),
	wOldValue(wOldValue),
	wNewValue(wNewValue)
{}

void ChangeLayerProperty::apply() {
	properties->setLayerId(layerId);
	property->setValueFromString(wNewValue);
	properties->applyChange();
}

void ChangeLayerProperty::revert() {
	properties->setLayerId(layerId);
	property->setValueFromString(wOldValue);
	properties->applyChange();
}
