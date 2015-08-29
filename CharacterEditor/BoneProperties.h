#pragma once

#include <unordered_set>

#include "../Calamity/Properties.h"
#include "../Calamity/ActionManager.h"
#include "BoneSelection.h"

#include "../Calamity/Gui/ListView.h"

#include "Actions/ChangeBoneProperty.h"
/*#include "ActionHandler/MoveSpritesHandler.h"
#include "ActionHandler/ScaleSpritesHandler.h"
#include "ActionHandler/RotateSpritesHandler.h"*/

using namespace clm;
using std::unordered_map;
class BoneSelection;

class BoneProperties : public Properties {
private:
	/*enum ActionType {
		MOVE_X,
		MOVE_Y,
		SCALE,
		ROTATE,
	};*/

	ActionManager* actionManager;
	BoneManager* boneManager;
	BoneSelection* selection;

	//unordered_map<Property*, ActionType> propertiesWithCustomAction;

	unsigned int boneId;

	StringProperty boneName;
	/*FloatProperty x;
	FloatProperty y;*/

	Functor* loadBoneListFunctor;
public:
	BoneProperties(ActionManager* actionManager, BoneManager* boneManager, BoneSelection* selection);
	void setLoadBoneListFunctor(Functor* loadBoneListFunctor);
	/*void setBoneList(ListView* boneList);
	ListView* getBoneList();*/
	void setBoneId(unsigned int boneId);
	void updateProperties();
	void pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue);
	void applyChange();

	StringProperty* getBoneNameProperty();
	/*FloatProperty* getXProperty();
	FloatProperty* getYProperty();*/
};
