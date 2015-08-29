#include "CharacterEditorProperties.h"

CharacterEditorProperties::CharacterEditorProperties(SpriteManager* spriteManager, ActionManager* actionManager, BoneManager* boneManager, BoneSelection* selection) :
	spriteManager(spriteManager),
	window(nullptr),
	fontId(-1),
	//boneList(nullptr),
	loadBoneListFunctor(),
	raiseBoneFunctor(),
	lowerBoneFunctor(),
	boneProperties(actionManager, boneManager, selection)
	/*showBones(),
	showRigidBodies(),
	showConstraints(),
	showGBuffer()*/
{
	characterEditorMode.updateValue(BONES_MODE);

	char buffer[3];
	snprintf(buffer, 3, "%d", SKIN_MODE);
	string sValue = buffer;
	previousCharacterEditorMode.assign(sValue.begin(), sValue.end());

	loadBoneListFunctor.reset(new LoadBoneListFunctor(boneManager, selection, actionManager, spriteManager));
	raiseBoneFunctor.reset(new ReorderBoneFunctor(actionManager, boneManager, loadBoneListFunctor.get(), true));
	lowerBoneFunctor.reset(new ReorderBoneFunctor(actionManager, boneManager, loadBoneListFunctor.get(), false));

	boneProperties.setLoadBoneListFunctor(loadBoneListFunctor.get());
}

BoneProperties* CharacterEditorProperties::getBoneProperties() {
	return &boneProperties;
}

LoadBoneListFunctor* CharacterEditorProperties::getLoadBoneListFunctor() {
	return loadBoneListFunctor.get();
}

void CharacterEditorProperties::updateProperties() {
}

void CharacterEditorProperties::pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue) {
	if (property == &characterEditorMode) {
		if (wNewValue.compare(wOldValue) != 0)
			previousCharacterEditorMode = wOldValue;
	}

	property->setValueFromString(wNewValue);
	applyChange();

	//if (propertiesWithCustomAction.find(property) == propertiesWithCustomAction.end()) {
		// TODO
		/*ChangeBoneProperty* action = new ChangeBoneProperty(this, boneId, property, wOldValue, wNewValue);
		actionManager->pushAction(action);*/
	/*}
	else {
		switch (propertiesWithCustomAction[property]) {
			case MOVE_X:
			{
				float oldValue = ((FloatProperty*)property)->getValue();
				property->setValueFromString(wNewValue);
				applyChange();
				float deltaX = ((FloatProperty*)property)->getValue() - oldValue;

				MoveSprites* action = new MoveSprites(selection);
				action->setDeltaPosition(Vec2(deltaX, 0));
				actionManager->pushAction(action);

				break;
				}
			case MOVE_Y:
			{
				float oldValue = ((FloatProperty*)property)->getValue();
				property->setValueFromString(wNewValue);
				applyChange();
				float deltaY = ((FloatProperty*)property)->getValue() - oldValue;

				MoveSprites* action = new MoveSprites(selection);
				action->setDeltaPosition(Vec2(0, deltaY));
				actionManager->pushAction(action);

				break;
			}
			case SCALE:
			{
				property->setValueFromString(wNewValue);
				applyChange();
				float scaleFactor = ((FloatProperty*)property)->getValue();

				ScaleSprites* action = new ScaleSprites(selection, true);
				action->setScaleFactor(scaleFactor);
				actionManager->pushAction(action);

				break;
			}
			case ROTATE:
			{
				property->setValueFromString(wNewValue);
				applyChange();
				float angle = ((FloatProperty*)property)->getValue();

				RotateSprites* action = new RotateSprites(selection, true);
				action->setRotationAngle(angle);
				actionManager->pushAction(action);

				break;
			}
		}
	}*/
}

void CharacterEditorProperties::applyChange() {
	if (characterEditorMode.hasChanged()) {
		characterEditorMode.setUnchanged();
		if (characterEditorMode.getValue() == BONES_MODE)
			loadBonesPanel();
		else if (characterEditorMode.getValue() == SKIN_MODE)
			loadSkinPanel();
		else if (characterEditorMode.getValue() == POSE_MODE)
			loadPosePanel();
		return;
	}
	/*if (showBones.hasChanged()) {
		if (character)
			character->setShowBones(showBones.getValue());
		showBones.setUnchanged();
		return;
	}
	if (showRigidBodies.hasChanged()) {
		if (debugDraw) {
			int debugMode = 0;
			if (showRigidBodies.getValue())
				debugMode = 1;
			debugDraw->setDebugMode(debugMode);
		}
		showRigidBodies.setUnchanged();
		return;
	}
	if (showConstraints.hasChanged()) {
		if (character)
			character->setShowConstraints(showConstraints.getValue());
		showConstraints.setUnchanged();
		return;
	}
	if (showGBuffer.hasChanged()) {
		showGBuffer.setUnchanged();
		return;
	}*/
}

void CharacterEditorProperties::setWindow(Window* window) {
	this->window = window;
}

void CharacterEditorProperties::setFontId(unsigned int fontId) {
	this->fontId = fontId;
	loadBoneListFunctor->setFontId(fontId);
}

void CharacterEditorProperties::loadBonesPanel() {
	window->clear();

	VerticalBoxLayout* boxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	window->setChild(boxLayout);

	VerticalLayout* bonesLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int boneListBox = boxLayout->addBox();
	boxLayout->setBoxWidget(boneListBox, bonesLayout);

	//ListView* boneList = new ListView(spriteManager, 100, 100, true, true);
	ListView* boneList = new ListView(spriteManager, 100, 100, true, true);
	bonesLayout->addChild(boneList);

	//loadBoneListFunctor.reset(new LoadBoneListFunctor(boneList, &boneManager, &boneSelection, &actionManager, spriteManager, fontId));
	loadBoneListFunctor->setBoneList(boneList);
	boneList->setLoadFunctor(loadBoneListFunctor.get());
	boneList->reload();

	ButtonGroup* boneButtons = new ButtonGroup(spriteManager, fontId, 150, true);
	bonesLayout->addChild(boneButtons);
	boneButtons->addRow();
	boneButtons->addButton(0, L"Move up", raiseBoneFunctor.get());
	boneButtons->addButton(0, L"Move down", lowerBoneFunctor.get());

	LineEdit* boneNameEdit = new LineEdit(spriteManager, 100, L"Bone", fontId, &boneProperties, boneProperties.getBoneNameProperty(), true);
	bonesLayout->addChild(boneNameEdit);

	/*LineEdit* boneNameEdit = new LineEdit(spriteManager, 100, L"Bone", fontId, true);
	bonesLayout->addChild(boneNameEdit);*/

	VerticalLayout* skinsLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int skinListBox = boxLayout->addBox();
	boxLayout->setBoxWidget(skinListBox, skinsLayout);

	ListView* skinList = new ListView(spriteManager, 100, 100, true, true);
	skinsLayout->addChild(skinList);

	ButtonGroup* skinButtons = new ButtonGroup(spriteManager, fontId, 150, true);
	skinsLayout->addChild(skinButtons);
	skinButtons->addRow();
	skinButtons->addButton(0, L"Add skin");
	skinButtons->addButton(0, L"Remove skin");

	/*LineEdit* skinNameEdit = new LineEdit(spriteManager, 100, L"Skin", fontId, true);
	skinsLayout->addChild(skinNameEdit);*/
}

void CharacterEditorProperties::loadSkinPanel() {
	window->clear();
	loadBoneListFunctor->setBoneList(nullptr);
}

void CharacterEditorProperties::loadPosePanel() {
	window->clear();
	loadBoneListFunctor->setBoneList(nullptr);
}

IntProperty* CharacterEditorProperties::getCharacterEditorModeProperty() {
	return &characterEditorMode;
}

wstring CharacterEditorProperties::getPreviousCharacterEditorMode() {
	return previousCharacterEditorMode;
}

/*BoolProperty* CharacterEditorProperties::getShowBonesProperty() {
	return &showBones;
}

BoolProperty* CharacterEditorProperties::getShowRigidBodiesProperty() {
	return &showRigidBodies;
}

BoolProperty* CharacterEditorProperties::getShowConstraintsProperty() {
	return &showConstraints;
}

BoolProperty* CharacterEditorProperties::getShowGBufferProperty() {
	return &showGBuffer;
}*/
