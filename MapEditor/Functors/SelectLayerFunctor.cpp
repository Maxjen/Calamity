#include "SelectLayerFunctor.h"

SelectLayerFunctor::SelectLayerFunctor(GameManager* gameManager, LayerManager* layerManager, LayerProperties* layerProperties, SpriteManager* spriteManager, VerticalBoxLayout* boxLayout, unsigned int boxId, unsigned int fontId) {
	this->gameManager = gameManager;
	this->layerManager = layerManager;
	this->layerProperties = layerProperties;
	this->spriteManager = spriteManager;
	this->boxLayout = boxLayout;
	this->boxId = boxId;
	this->fontId = fontId;
}

void SelectLayerFunctor::operator()() {
	boxLayout->clearBox(boxId);

	unsigned int selectedLayerId = layerManager->getSelectedLayerId();
	layerProperties->setLayerId(selectedLayerId);

	if (selectedLayerId != (unsigned int)-1) {
		VerticalLayout* layerPropertiesLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
		boxLayout->setBoxWidget(boxId, layerPropertiesLayout);
		LineEdit* layerNameEdit = new LineEdit(spriteManager, 100, L"Layer", fontId, layerProperties, layerProperties->getLayerNameProperty(), true);
		layerPropertiesLayout->addChild(layerNameEdit);

		InputGroup* scrollSpeedInput = new InputGroup(spriteManager, fontId, 150, true);
		layerPropertiesLayout->addChild(scrollSpeedInput);
		//boxLayout->setBoxWidget(boxId, scrollSpeedInput);
		scrollSpeedInput->addRow();
		scrollSpeedInput->addInput(0, L"ScrollSpeed", layerProperties, layerProperties->getScrollSpeedProperty());
		//scrollSpeedInput->initialize();

		layerPropertiesLayout->initialize();
	}
}
