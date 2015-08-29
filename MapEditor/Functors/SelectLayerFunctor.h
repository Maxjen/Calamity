#pragma once

#include "Functor.h"
#include "ActionManager.h"
#include "../../Common/LayerManager.h"
#include "../LayerProperties.h"
#include "Gui/VerticalBoxLayout.h"
#include "Gui/VerticalLayout.h"
#include "Gui/InputGroup.h"
#include "Gui/LineEdit.h"

using namespace clm;

class SelectLayerFunctor : public Functor {
private:
	GameManager* gameManager;
	LayerManager* layerManager;
	LayerProperties* layerProperties;
	SpriteManager* spriteManager;
	VerticalBoxLayout* boxLayout;
	unsigned int boxId;
	unsigned int fontId;
public:
	SelectLayerFunctor(GameManager* gameManager, LayerManager* layerManager, LayerProperties* layerProperties, SpriteManager* spriteManager, VerticalBoxLayout* boxLayout, unsigned int boxId, unsigned int fontId);
	void operator()();
};
