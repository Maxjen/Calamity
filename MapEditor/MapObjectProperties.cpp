#include "MapObjectProperties.h"

MapObjectProperties::MapObjectProperties(ActionManager* actionManager, MapEditorSelection* selection) :
	actionManager(actionManager),
	selection(selection),
	propertiesWithCustomAction(),
	x(),
	y(),
	scale(),
	rotation()
{
	propertiesWithCustomAction[&x] = MOVE_X;
	propertiesWithCustomAction[&y] = MOVE_Y;
	propertiesWithCustomAction[&scale] = SCALE;
	propertiesWithCustomAction[&rotation] = ROTATE;
}

void MapObjectProperties::updateProperties() {
	GameManager* gameManager = selection->getGameManager();
	/*list<unsigned int> selectedObjects = selection->getSelectedSprites();
	if (selectedObjects.size() == 1) {*/
	unsigned int selectedObject = selection->getMainSelectedSprite();
	if (selectedObject != (unsigned int)-1) {
		Vec2 pos = ((SpriteObject*)gameManager->getObject(selectedObject))->getPosition();
		x.updateValue(pos.x);
		y.updateValue(pos.y);

		float scaleFactor = ((SpriteObject*)gameManager->getObject(selectedObject))->getScale();
		scale.updateValue(scaleFactor);

		float angle = ((SpriteObject*)gameManager->getObject(selectedObject))->getRotation();
		rotation.updateValue(angle);
	}
}

void MapObjectProperties::pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue) {
	if (propertiesWithCustomAction.find(property) == propertiesWithCustomAction.end()) {
		// TODO
		/*ChangeLayerProperty* action = new ChangeLayerProperty(this, layerId, property, wOldValue, wNewValue);
		actionManager->pushAction(action);*/
	}
	else {
		switch (propertiesWithCustomAction[property]) {
			case MOVE_X:
			{
				float oldValue = ((FloatProperty*)property)->getValue();
				property->setValueFromString(wNewValue);
				applyChange();
				float deltaX = ((FloatProperty*)property)->getValue() - oldValue;

				MoveObjects* action = new MoveObjects(selection, true);
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

				MoveObjects* action = new MoveObjects(selection, true);
				action->setDeltaPosition(Vec2(0, deltaY));
				actionManager->pushAction(action);

				break;
			}
			case SCALE:
			{
				property->setValueFromString(wNewValue);
				applyChange();
				float scaleFactor = ((FloatProperty*)property)->getValue();

				ScaleObjects* action = new ScaleObjects(selection, true);
				action->setAbsoluteScale(true);
				action->setScaleFactor(scaleFactor);
				actionManager->pushAction(action);

				break;
			}
			case ROTATE:
			{
				property->setValueFromString(wNewValue);
				applyChange();
				float angle = ((FloatProperty*)property)->getValue();

				RotateObjects* action = new RotateObjects(selection, true);
				action->setAbsoluteRotation(true);
				action->setRotationAngle(angle);
				actionManager->pushAction(action);

				break;
			}
		}
	}
}

void MapObjectProperties::applyChange() {
	if (x.hasChanged()) {
		x.setUnchanged();
		return;
	}
	if (y.hasChanged()) {
		y.setUnchanged();
		return;
	}
	if (scale.hasChanged()) {
		scale.setUnchanged();
		return;
	}
	if (rotation.hasChanged()) {
		rotation.setUnchanged();
		return;
	}
}

FloatProperty* MapObjectProperties::getXProperty() {
	return &x;
}

FloatProperty* MapObjectProperties::getYProperty() {
	return &y;
}

FloatProperty* MapObjectProperties::getScaleProperty() {
	return &scale;
}

FloatProperty* MapObjectProperties::getRotationProperty() {
	return &rotation;
}
