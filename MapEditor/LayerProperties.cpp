#include "LayerProperties.h"

LayerProperties::LayerProperties(ActionManager* actionManager, LayerManager* layerManager, GameManager* gameManager) :
	actionManager(actionManager),
	layerManager(layerManager),
	layerList(nullptr),
	gameManager(gameManager),
	layerId(-1),
	layerName(),
	scrollSpeed()
{}

void LayerProperties::setLayerList(ListView* layerList) {
	this->layerList = layerList;
}

ListView* LayerProperties::getLayerList() {
	return layerList;
}

void LayerProperties::setLayerId(unsigned int layerId) {
	this->layerId = layerId;
	layerManager->setSelectedLayerId(layerId);

	if (layerId != (unsigned int)-1) {
		scrollSpeed.updateValue(gameManager->getLayerScrollSpeed(layerId));
		layerName.updateValue(layerManager->getLayerName(layerId));
	}
	else {
		layerName.updateValue(L"");
	}
}

void LayerProperties::pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue) {
	if (layerId != (unsigned int)-1) {
		ChangeLayerProperty* action = new ChangeLayerProperty(this, layerId, property, wOldValue, wNewValue);
		actionManager->pushAction(action);
	}
}

void LayerProperties::applyChange() {
	if (layerName.hasChanged()) {
		layerManager->setLayerName(layerId, layerName.getValueAsString());
		if (layerList)
			layerList->reload();
		layerName.setUnchanged();
		return;
	}
	if (scrollSpeed.hasChanged()) {
		gameManager->setLayerScrollSpeed(layerId, scrollSpeed.getValue());
		scrollSpeed.setUnchanged();
		return;
	}
}

StringProperty* LayerProperties::getLayerNameProperty() {
	return &layerName;
}

FloatProperty* LayerProperties::getScrollSpeedProperty() {
	return &scrollSpeed;
}
