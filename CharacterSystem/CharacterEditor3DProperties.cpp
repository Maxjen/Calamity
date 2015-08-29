#include "CharacterEditor3DProperties.h"

CharacterEditor3DProperties::CharacterEditor3DProperties(SpriteManager* spriteManager) :
	spriteManager(spriteManager),
	window(nullptr),
	fontId(-1),
	character(nullptr),
	debugDraw(nullptr),
	showBones(),
	showPhysicsDebug(),
	showConstraints(),
	showGBuffer(),
	linStiffness(),
	angStiffness(),
	damping(),
	drag(),
	lift(),
	dynamicFriction(),
	totalMass(),
	maxDistance()
{
	characterEditor3DMode.updateValue(CHARACTER_MODE);

	char buffer[3];
	snprintf(buffer, 3, "%d", PHYSICS_MODE);
	string sValue = buffer;
	previousCharacterEditor3DMode.assign(sValue.begin(), sValue.end());

	linStiffness.updateValue(0.4f);
	angStiffness.updateValue(1.0f);
	damping.updateValue(0.05f);
	drag.updateValue(0.5f);
	lift.updateValue(10.0f);
	dynamicFriction.updateValue(1.0f);
	totalMass.updateValue(1.0f);
	maxDistance.updateValue(0.3f);
}

void CharacterEditor3DProperties::setCharacter(CharacterObject* character) {
	this->character = character;
}

void CharacterEditor3DProperties::setDebugDraw(DebugDraw* debugDraw) {
	this->debugDraw = debugDraw;
}

void CharacterEditor3DProperties::updateProperties() {
}

void CharacterEditor3DProperties::pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue) {
	if (property == &characterEditor3DMode) {
		if (wNewValue.compare(wOldValue) != 0)
			previousCharacterEditor3DMode = wOldValue;
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

void CharacterEditor3DProperties::applyChange() {
	if (characterEditor3DMode.hasChanged()) {
		characterEditor3DMode.setUnchanged();
		if (characterEditor3DMode.getValue() == CHARACTER_MODE)
			loadCharacterPanel();
		else if (characterEditor3DMode.getValue() == PHYSICS_MODE)
			loadPhysicsPanel();
		return;
	}
	if (showBones.hasChanged()) {
		if (character)
			character->setShowBones(showBones.getValue());
		showBones.setUnchanged();
		return;
	}
	if (showPhysicsDebug.hasChanged()) {
		if (debugDraw) {
			int debugMode = 0;
			if (showPhysicsDebug.getValue())
				debugMode = 1;
			debugDraw->setDebugMode(debugMode);
		}
		showPhysicsDebug.setUnchanged();
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
	}
	if (linStiffness.hasChanged()) {
		character->reloadClothComponents();
		linStiffness.setUnchanged();
		return;
	}
	if (angStiffness.hasChanged()) {
		character->reloadClothComponents();
		angStiffness.setUnchanged();
		return;
	}
	if (damping.hasChanged()) {
		character->reloadClothComponents();
		damping.setUnchanged();
		return;
	}
	if (drag.hasChanged()) {
		character->reloadClothComponents();
		drag.setUnchanged();
		return;
	}
	if (lift.hasChanged()) {
		character->reloadClothComponents();
		lift.setUnchanged();
		return;
	}
	if (dynamicFriction.hasChanged()) {
		character->reloadClothComponents();
		dynamicFriction.setUnchanged();
		return;
	}
	if (totalMass.hasChanged()) {
		character->reloadClothComponents();
		totalMass.setUnchanged();
		return;
	}
	if (maxDistance.hasChanged()) {
		character->reloadClothComponents();
		maxDistance.setUnchanged();
		return;
	}
}

void CharacterEditor3DProperties::setWindow(Window* window) {
	this->window = window;
}

void CharacterEditor3DProperties::setFontId(unsigned int fontId) {
	this->fontId = fontId;
}

void CharacterEditor3DProperties::loadCharacterPanel() {
	window->clear();

	VerticalBoxLayout* boxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	window->setChild(boxLayout);

	ListView* characterComponentList = new ListView(spriteManager, 100, 100, true, true);
	unsigned int characterComponentListBox = boxLayout->addBox();
	boxLayout->setBoxWidget(characterComponentListBox, characterComponentList);

	{
		DIR *dp;
		struct dirent *dirp;
		//dp  = opendir((string("/home/max/projects/mt/build/").append(folderName)).c_str());
		dp  = opendir(string("Data/MeshComponents").c_str());
		if (dp == NULL)
			printf("Unable to open Data/MeshComponents");

		list<string> fileNames;
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_name[0] != '.') {
				string fileName = dirp->d_name;
				fileNames.push_back(fileName);
			}
		}
		closedir(dp);
		fileNames.sort();
		for (auto& it : fileNames) {
			CharacterComponentItem* item = new CharacterComponentItem(spriteManager, character, it.c_str(), fontId);
			characterComponentList->addItem(item);
		}
	}

	ListView* clothComponentList = new ListView(spriteManager, 100, 100, true, true);
	unsigned int clothComponentListBox = boxLayout->addBox();
	boxLayout->setBoxWidget(clothComponentListBox, clothComponentList);

	{
		DIR *dp;
		struct dirent *dirp;
		//dp  = opendir((string("/home/max/projects/mt/build/").append(folderName)).c_str());
		dp  = opendir(string("Data/ClothComponents").c_str());
		if (dp == NULL)
			printf("Unable to open Data/ClothComponents");

		list<string> fileNames;
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_name[0] != '.') {
				string fileName = dirp->d_name;
				fileNames.push_back(fileName);
			}
		}
		closedir(dp);
		fileNames.sort();
		for (auto& it : fileNames) {
			CharacterComponentItem* item = new CharacterComponentItem(spriteManager, character, it.c_str(), fontId, true);
			clothComponentList->addItem(item);
		}
	}

	VerticalLayout* viewOptionsLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int viewOptionsBox = boxLayout->addBox();
	boxLayout->setBoxWidget(viewOptionsBox, viewOptionsLayout);

	Checkbox* checkShowBones = new Checkbox(spriteManager, 100, L"show bones", fontId, this, &showBones, true);
	viewOptionsLayout->addChild(checkShowBones);
}

void CharacterEditor3DProperties::loadPhysicsPanel() {
	window->clear();

	VerticalBoxLayout* boxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	window->setChild(boxLayout);

	VerticalLayout* viewOptionsLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int viewOptionsBox = boxLayout->addBox();
	boxLayout->setBoxWidget(viewOptionsBox, viewOptionsLayout);

	Checkbox* checkShowPhysicsDebugBodies = new Checkbox(spriteManager, 100, L"Physics debug draw", fontId, this, &showPhysicsDebug, true);
	viewOptionsLayout->addChild(checkShowPhysicsDebugBodies);

	Checkbox* checkShowConstraints = new Checkbox(spriteManager, 100, L"show constraints", fontId, this, &showConstraints, true);
	viewOptionsLayout->addChild(checkShowConstraints);


	InputGroup* clothAttributesInput = new InputGroup(spriteManager, fontId, 150, true);
	viewOptionsLayout->addChild(clothAttributesInput);
	clothAttributesInput->addRow();
	clothAttributesInput->addInput(0, L"Linear Stiffness", this, &linStiffness);
	clothAttributesInput->addRow();
	clothAttributesInput->addInput(1, L"Angular Stiffness", this, &angStiffness);
	clothAttributesInput->addRow();
	clothAttributesInput->addInput(2, L"Damping", this, &damping);
	clothAttributesInput->addRow();
	clothAttributesInput->addInput(3, L"Drag", this, &drag);
	clothAttributesInput->addRow();
	clothAttributesInput->addInput(4, L"Lift", this, &lift);
	clothAttributesInput->addRow();
	clothAttributesInput->addInput(5, L"Dynamic Friction", this, &dynamicFriction);
	clothAttributesInput->addRow();
	clothAttributesInput->addInput(6, L"Total mass", this, &totalMass);
	clothAttributesInput->addRow();
	clothAttributesInput->addInput(7, L"Maximum distance", this, &maxDistance);
	clothAttributesInput->initialize();
}

IntProperty* CharacterEditor3DProperties::getCharacterEditor3DModeProperty() {
	return &characterEditor3DMode;
}

wstring CharacterEditor3DProperties::getPreviousCharacterEditor3DMode() {
	return previousCharacterEditor3DMode;
}

BoolProperty* CharacterEditor3DProperties::getShowBonesProperty() {
	return &showBones;
}

BoolProperty* CharacterEditor3DProperties::getShowPhysicsDebugProperty() {
	return &showPhysicsDebug;
}

BoolProperty* CharacterEditor3DProperties::getShowConstraintsProperty() {
	return &showConstraints;
}

BoolProperty* CharacterEditor3DProperties::getShowGBufferProperty() {
	return &showGBuffer;
}

float CharacterEditor3DProperties::getLinStiffness() {
	return linStiffness.getValue();
}

float CharacterEditor3DProperties::getAngStiffness() {
	return angStiffness.getValue();
}

float CharacterEditor3DProperties::getDamping() {
	return damping.getValue();
}

float CharacterEditor3DProperties::getDrag() {
	return drag.getValue();
}

float CharacterEditor3DProperties::getLift() {
	return lift.getValue();
}

float CharacterEditor3DProperties::getDynamicFriction() {
	return dynamicFriction.getValue();
}

float CharacterEditor3DProperties::getTotalMass() {
	return totalMass.getValue();
}

float CharacterEditor3DProperties::getMaxDistance() {
	return maxDistance.getValue();
}
