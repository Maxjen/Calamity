#pragma once

#include "../Calamity/Properties.h"

#include "Gui/WindowManager.h"
#include "Gui/VerticalBoxLayout.h"
#include "Gui/VerticalLayout.h"
#include "Gui/HorizontalLayout.h"
#include "Gui/SegmentControl.h"
#include "Gui/ButtonGroup.h"
#include "Gui/InputGroup.h"
#include "Gui/LineEdit.h"
#include "Gui/Checkbox.h"
#include "Gui/CharacterComponentItem.h"

#include "CharacterObject.h"

using namespace clm;

class CharacterObject;
class DebugDraw;

enum CharacterEditor3DMode {
	CHARACTER_MODE,
	PHYSICS_MODE,
	PLAY_MODE,
};

class CharacterEditor3DProperties : public Properties {
private:
	SpriteManager* spriteManager;
	clm::Window* window;
	unsigned int fontId;

	CharacterObject* character;
	DebugDraw* debugDraw;

	IntProperty characterEditor3DMode;
	wstring previousCharacterEditor3DMode;

	BoolProperty showBones;
	BoolProperty showPhysicsDebug;
	BoolProperty showConstraints;
	BoolProperty showGBuffer;

	FloatProperty linStiffness;
	FloatProperty angStiffness;
	FloatProperty damping;
	FloatProperty drag;
	FloatProperty lift;
	FloatProperty dynamicFriction;
	FloatProperty totalMass;
	FloatProperty maxDistance;
public:
	CharacterEditor3DProperties(SpriteManager* spriteManager);
	void setCharacter(CharacterObject* character);
	void setDebugDraw(DebugDraw* debugDraw);
	void updateProperties();
	void pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue);
	void applyChange();

	void setWindow(clm::Window* window);
	void setFontId(unsigned int fontId);
	void loadCharacterPanel();
	void loadPhysicsPanel();

	IntProperty* getCharacterEditor3DModeProperty();
	wstring getPreviousCharacterEditor3DMode();

	BoolProperty* getShowBonesProperty();
	BoolProperty* getShowPhysicsDebugProperty();
	BoolProperty* getShowConstraintsProperty();
	BoolProperty* getShowGBufferProperty();

	float getLinStiffness();
	float getAngStiffness();
	float getDamping();
	float getDrag();
	float getLift();
	float getDynamicFriction();
	float getTotalMass();
	float getMaxDistance();
};
