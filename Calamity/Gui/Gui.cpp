#include "Gui.h"

namespace clm {

Gui::Gui(SpriteManager* spriteManager, SpriteRenderer* spriteRenderer, int screenWidth, int screenHeight) {
	this->spriteManager = spriteManager;
	this->spriteRenderer = spriteRenderer;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

Gui::~Gui() {
}

Window* Gui::createWidow(int posX, int posY, int width, int height) {

}

void Gui::deleteWindow() {

}

}
