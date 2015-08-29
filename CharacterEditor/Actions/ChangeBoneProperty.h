#pragma once

#include "../../Calamity/Action.h"
#include "../BoneProperties.h"

using namespace clm;

class BoneProperties;

class ChangeBoneProperty : public Action {
private:
	BoneProperties* properties;
	unsigned int boneId;
	Property* property;
	wstring wOldValue;
	wstring wNewValue;
public:
	explicit ChangeBoneProperty(BoneProperties* properties, unsigned int boneId, Property* property,
						wstring wOldValue, wstring wNewValue);
    void apply();
    void revert();
};
