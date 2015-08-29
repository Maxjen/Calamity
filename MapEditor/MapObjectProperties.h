#pragma once

#include <unordered_set>

#include "../Calamity/Properties.h"
#include "../Calamity/ActionManager.h"
#include "MapEditorSelection.h"

#include "Actions/MoveObjects.h"
#include "Actions/ScaleObjects.h"
#include "Actions/RotateObjects.h"

using namespace clm;
using std::unordered_map;
class SpriteSelection;

class MapObjectProperties : public Properties {
private:
	enum ActionType {
		MOVE_X,
		MOVE_Y,
		SCALE,
		ROTATE,
	};

	ActionManager* actionManager;
	MapEditorSelection* selection;

	unordered_map<Property*, ActionType> propertiesWithCustomAction;

	FloatProperty x;
	FloatProperty y;
	FloatProperty scale;
	FloatProperty rotation;
public:
	explicit MapObjectProperties(ActionManager* actionManager, MapEditorSelection* selection);
	void updateProperties();
	void pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue);
	void applyChange();

	FloatProperty* getXProperty();
	FloatProperty* getYProperty();
	FloatProperty* getScaleProperty();
	FloatProperty* getRotationProperty();
};
