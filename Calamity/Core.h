#pragma once

#include <SDL2/SDL.h>
#include <GL/glew.h>

//#include <EGL/egl.h>
//#include <GL/gl.h>
//#include "GL3/gl3.h"

//#include <glm/glm.hpp>

#include "ContextManager.h"

namespace clm {

class Core {
private:
	SDL_Window* mainwindow; // Our window handle
	SDL_GLContext maincontext; // Our opengl context handle

	ContextManager contextManager;
	//Context* currentContext;

	int screenWidth, screenHeight;
	bool isFullscreen;
public:
	explicit Core();
	~Core();

	ContextManager* getContextManager();

	unsigned int addContext(Context* context);
	void setCurrentContext(unsigned int contextId);
	//void setCurrentContext(Context* context);

	int getScreenWidth();
	int getScreenHeight();

	int init(const char* windowTitle);
	void mainLoop();

};

}
