#ifndef GUI_H
#define GUI_H

#include "../Renderer/RenderLayer.h"
#include "../SpriteManager.h"

namespace clm {

class Gui {
private:
	SpriteManager* spriteManager;
	SpriteRenderer* spriteRenderer;
	int screenWidth, screenHeight;
public:
	Gui(SpriteManager* spriteManager, SpriteRenderer* spriteRenderer, int screenWidth, int screenHeight);
	~Gui();

	Window* createWindow(int posX, int posY, int width, int height);
	void deleteWindow(Window* window);

	virtual void draw();
};

}

#endif // GUI_H
