#include "ChangeBoneProperty.h"

ChangeBoneProperty::ChangeBoneProperty(BoneProperties* properties, unsigned int boneId, Property* property,
										 wstring wOldValue, wstring wNewValue) :
	properties(properties),
	boneId(boneId),
	property(property),
	wOldValue(wOldValue),
	wNewValue(wNewValue)
{}

void ChangeBoneProperty::apply() {
	properties->setBoneId(boneId);
	property->setValueFromString(wNewValue);
	properties->applyChange();
}

void ChangeBoneProperty::revert() {
	properties->setBoneId(boneId);
	property->setValueFromString(wOldValue);
	properties->applyChange();
}
