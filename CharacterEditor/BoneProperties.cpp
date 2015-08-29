#include "BoneProperties.h"

BoneProperties::BoneProperties(ActionManager* actionManager, BoneManager* boneManager, BoneSelection* selection) :
	actionManager(actionManager),
	boneManager(boneManager),
	selection(selection),
	boneId(-1),
	loadBoneListFunctor(nullptr)
{
	//boneList = nullptr;

	/*propertiesWithCustomAction[&x] = MOVE_X;
	propertiesWithCustomAction[&y] = MOVE_Y;
	propertiesWithCustomAction[&scale] = SCALE;
	propertiesWithCustomAction[&rotation] = ROTATE;*/
}

void BoneProperties::setLoadBoneListFunctor(Functor* loadBoneListFunctor) {
	this->loadBoneListFunctor = loadBoneListFunctor;
}

/*void BoneProperties::setBoneList(ListView* boneList) {
	this->boneList = boneList;
}

ListView* BoneProperties::getBoneList() {
	return boneList;
}*/

void BoneProperties::setBoneId(unsigned int boneId) {
	this->boneId = boneId;
	boneManager->setSelectedBoneId(boneId);

	if (boneId != (unsigned int)-1) {
		boneName.updateValue(boneManager->getBoneName(boneId));
	}
	else {
		boneName.updateValue(L"");
	}
}

void BoneProperties::updateProperties() {
	//GameManager* gameManager = selection->getGameManager();


	/*unordered_map<unsigned int, int> selectedBones = selection->getSelectedBones();
	if (selectedBones.size() == 1) {*/
	unsigned int selectedBone = selection->getMainSelectedBone();
	if (selectedBone != (unsigned int)-1) {
		boneName.updateValue(boneManager->getBoneName(selectedBone));
		/*Vec2 pos = ((SpriteObject*)gameManager->getObject(*(selectedObjects.begin())))->getPosition();
		x.updateValue(pos.x);
		y.updateValue(pos.y);

		float scaleFactor = ((SpriteObject*)gameManager->getObject(*(selectedObjects.begin())))->getScale();
		scale.updateValue(scaleFactor);

		float angle = ((SpriteObject*)gameManager->getObject(*(selectedObjects.begin())))->getRotation();
		rotation.updateValue(angle);*/
	}
	else {
		boneName.updateValue(L"");
	}
}

void BoneProperties::pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue) {
	//if (propertiesWithCustomAction.find(property) == propertiesWithCustomAction.end()) {
		// TODO
		ChangeBoneProperty* action = new ChangeBoneProperty(this, boneId, property, wOldValue, wNewValue);
		actionManager->pushAction(action);
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

void BoneProperties::applyChange() {
	if (boneName.hasChanged()) {
		boneManager->setBoneName(boneId, boneName.getValueAsString());
		if (loadBoneListFunctor != nullptr)
			(*loadBoneListFunctor)();
		boneName.setUnchanged();
		return;
	}
	/*if (x.hasChanged()) {
		x.setUnchanged();
		return;
	}
	if (y.hasChanged()) {
		y.setUnchanged();
		return;
	}*/
}

StringProperty* BoneProperties::getBoneNameProperty() {
	return &boneName;
}

/*FloatProperty* BoneProperties::getXProperty() {
	return &x;
}

FloatProperty* BoneProperties::getYProperty() {
	return &y;
}*/
