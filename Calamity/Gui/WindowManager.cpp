#include "WindowManager.h"

namespace clm {

WindowManager::WindowManager(SpriteManager* spriteManager) :
	spriteManager(spriteManager),
	bottomLevelShadowLayer(spriteManager->getRenderer()),

	windows(),
	cWindow(0),

	windowsByZOrder()
{}

unsigned int WindowManager::createWindow(int posX, int posY, int width, int height, int flags) {
	//Window* window = new Window(cWindow, spriteManager, posX, posY, width, height, flags);
	unique_ptr<Window> window(new Window(cWindow, spriteManager, posX, posY, width, height, flags));
	if (flags & CLM_BOTTOM_LEVEL)
		window->setShadowLayer(&bottomLevelShadowLayer);
	window->setZOrder(cWindow);
	window->initialize();
	windowsByZOrder[cWindow] = window.get();
	windows[cWindow] = move(window);
	return cWindow++;
}

void WindowManager::deleteWindow(unsigned int windowId) {
	unsigned int zOrder = windows[windowId]->getZOrder();
	//delete windows[windowId];
	//windows[windowId].reset();
	windowsByZOrder.erase(zOrder);
	windows.erase(windowId);
}

Window* WindowManager::getWindow(unsigned int windowId) {
	return windows[windowId].get();
}

bool WindowManager::contains(int x, int y) {
	bool result = false;
	for (auto& it : windows) {
		if (it.second->contains(x, y)) {
			result = true;
			break;
		}
	}
	return result;
}

ActionHandler* WindowManager::createActionHandler(int x, int y, SDL_Event* event) {
	ActionHandler* result = nullptr;
	list<Window*> possibleWindows;

	for (auto& it : windows) {
		if (it.second->contains(x, y))
			possibleWindows.push_back(it.second.get());
	}

	if (!possibleWindows.empty()) {
		list<Window*>::iterator it = possibleWindows.begin();
		Window* topWindow = *it;
		++it;
		for (; it != possibleWindows.end(); ++it) {
			if ((*it)->getZOrder() > topWindow->getZOrder())
				topWindow = (*it);
		}
		result = topWindow->createActionHandler(x, y, event);

		// move topWindow to top
		if (!(topWindow->getFlags() & CLM_BOTTOM_LEVEL) && event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
			unsigned int lastZ = (*--(windowsByZOrder.end())).second->getZOrder();
			unsigned int nextZ = topWindow->getZOrder();
			unsigned int currentZ;
			map<unsigned int, Window*>::iterator nextWindow;
			while ((currentZ = topWindow->getZOrder()) < lastZ) {
				++nextZ;
				if ((nextWindow = windowsByZOrder.find(nextZ)) != windowsByZOrder.end()) {
					windowsByZOrder[currentZ] = windowsByZOrder[nextZ];
					windowsByZOrder[currentZ]->setZOrder(currentZ);
					windowsByZOrder[nextZ] = topWindow;
					topWindow->setZOrder(nextZ);
				}
			}
		}
	}

	return result;
}

void WindowManager::update() {
	bottomLevelShadowLayer.clearLayers();
	for (auto& it : windows)
		it.second->update();
}

void WindowManager::draw() {
	bottomLevelShadowLayer.uploadLayerBuffers();
	bottomLevelShadowLayer.draw();

	for (auto& it : windowsByZOrder)
		it.second->draw();
}

}
