#pragma once

#include "../Calamity/Properties.h"
#include "BoneProperties.h"
#include "Functors/LoadBoneListFunctor.h"
#include "Functors/ReorderBoneFunctor.h"

#include "Gui/WindowManager.h"
#include "Gui/VerticalBoxLayout.h"
#include "Gui/VerticalLayout.h"
#include "Gui/HorizontalLayout.h"
#include "Gui/SegmentControl.h"
#include "Gui/ButtonGroup.h"
#include "Gui/InputGroup.h"
#include "Gui/LineEdit.h"
#include "Gui/Checkbox.h"

using namespace clm;

enum CharacterEditorMode {
	BONES_MODE,
	SKIN_MODE,
	POSE_MODE,
};

class CharacterEditorProperties : public Properties {
private:
	SpriteManager* spriteManager;
	clm::Window* window;
	unsigned int fontId;

	//ListView* boneList;
	unique_ptr<LoadBoneListFunctor> loadBoneListFunctor;
	unique_ptr<ReorderBoneFunctor> raiseBoneFunctor;
	unique_ptr<ReorderBoneFunctor> lowerBoneFunctor;

	BoneProperties boneProperties;

	IntProperty characterEditorMode;
	wstring previousCharacterEditorMode;

	/*BoolProperty showBones;
	BoolProperty showRigidBodies;
	BoolProperty showConstraints;
	BoolProperty showGBuffer;*/
public:
	CharacterEditorProperties(SpriteManager* spriteManager, ActionManager* actionManager, BoneManager* boneManager, BoneSelection* selection);
	BoneProperties* getBoneProperties();
	LoadBoneListFunctor* getLoadBoneListFunctor();
	void updateProperties();
	void pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue);
	void applyChange();

	void setWindow(clm::Window* window);
	void setFontId(unsigned int fontId);
	void loadBonesPanel();
	void loadSkinPanel();
	void loadPosePanel();


	IntProperty* getCharacterEditorModeProperty();
	wstring getPreviousCharacterEditorMode();

	/*BoolProperty* getShowBonesProperty();
	BoolProperty* getShowRigidBodiesProperty();
	BoolProperty* getShowConstraintsProperty();
	BoolProperty* getShowGBufferProperty();*/
};
