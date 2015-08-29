#include "Core.h"

namespace clm {

Core::Core() :
	mainwindow(nullptr),
	maincontext(),
	contextManager(),
	screenWidth(),
	screenHeight(),
	isFullscreen(true)
{}

Core::~Core() {
	// Delete our opengl context, destroy our window, and shutdown SDL
	SDL_GL_DeleteContext(maincontext);
	SDL_DestroyWindow(mainwindow);
	SDL_Quit();
}

ContextManager* Core::getContextManager() {
	return &contextManager;
}

unsigned int Core::addContext(Context* context) {
	return contextManager.addContext(context);
}

void Core::setCurrentContext(unsigned int contextId) {
	contextManager.setActiveContext(contextId);
}

int Core::getScreenWidth() {
	return screenWidth;
}

int Core::getScreenHeight() {
	return screenHeight;
}

/*void Core::setCurrentContext(Context *context) {
	currentContext = context;
}*/

int Core::init(const char* windowTitle) {
	/*this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;*/

	// Request opengl 3.2 context.
	// SDL doesn't have the ability to choose which profile at this time of writing,
	// but it should default to the core profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return -1;
	}

	SDL_DisplayMode displayMode;
	if (SDL_GetDesktopDisplayMode(0, &displayMode) != 0) {
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return -1;
	}
	this->screenWidth = displayMode.w;
	this->screenHeight = displayMode.h;

	// Create our window centered with specified resolution
	mainwindow = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		displayMode.w, displayMode.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	if (!mainwindow) {
		printf("Unable to create window: %s\n", SDL_GetError());
		return -1;
	}

	// Create our opengl context and attach it to our window
	maincontext = SDL_GL_CreateContext(mainwindow);
	if (!maincontext) {
		printf("Unable to create context: %s\n", SDL_GetError());
		return -1;
	}

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Initialize glew
	GLenum status = glewInit();
	if(status != GLEW_OK) {
		printf("Unable to initialize glew: %s\n", glewGetErrorString(status));
		return -1;
	}

	/*int numberOfExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);
	for (int i = 0; i < numberOfExtensions; i++) {
		const GLubyte* ccc = glGetStringi(GL_EXTENSIONS, i);
		printf("%s\n", (char*)ccc);
		if ( strcmp(ccc, (const GLubyte *)"GL_ARB_debug_output") == 0 ){
			// The extension is supported by our hardware and driver
			// Try to get the "glDebugMessageCallbackARB" function :
			glDebugMessageCallbackARB  = (PFNGLDEBUGMESSAGECALLBACKARBPROC) wglGetProcAddress("glDebugMessageCallbackARB");
		}
	}*/

	glViewport(0, 0, screenWidth, screenHeight);

	// Set the color and depth clear values
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	//glClearColor(1.f, 1.f, 1.f, 1.f);

	// Enable Z-buffer read and write
	/*glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE); // enabled by default
	glDepthFunc(GL_LEQUAL);*/

	/*glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);*/

	return 0;
}

void Core::mainLoop() {
	//unsigned int oldTime = SDL_GetTicks();

	bool done = false;

	Context* activeContext = contextManager.getActiveContext();
	if (!activeContext) {
		printf("No context set! Exiting main loop.\n");
		done = true;
	}

	const int ticksPerSecond = 30;
	const int skipTicks = 1000 / ticksPerSecond;
	const int maxFrameSkip = 10;

	unsigned int nextTick = SDL_GetTicks() - 1;
	int loops;

	while (!done) {
		activeContext = contextManager.getActiveContext();

		loops = 0;
		while (SDL_GetTicks() > nextTick && loops < maxFrameSkip) {




			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				done = activeContext->handleEvent(&event);
				switch (event.type) {
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_F1) {
							if (!isFullscreen) {
								SDL_SetWindowFullscreen(mainwindow, SDL_WINDOW_FULLSCREEN);
								isFullscreen = true;
							}
							else {
								SDL_SetWindowFullscreen(mainwindow, 0);
								isFullscreen = false;
							}
						}
						break;
					case SDL_QUIT:
						done = true;
						break;
					default:
						break;
				}
			}

			//frameRender();
			activeContext->update(1.0f / ticksPerSecond);

			nextTick += skipTicks;
			++loops;
		}
		//printf("loops %d\n", loops);

		if (loops != 0) {
			activeContext->frameRender();

			// Swap our back buffer to the front
			SDL_GL_SwapWindow(mainwindow);
		}

		//SDL_Delay(20); // change this later
	}
}

/*void Core::mainLoop() {
	SDL_Event event;
	bool done = false;
	unsigned int oldTime = SDL_GetTicks();

	Context* activeContext = contextManager.getActiveContext();

	if (!activeContext) {
		printf("No context set! Exiting main loop.\n");
		done = true;
	}

	while (!done) {
		unsigned int newTime = SDL_GetTicks();
		float deltaTime = newTime - oldTime;
		deltaTime /= 1000.0f;
		oldTime = newTime;

		activeContext = contextManager.getActiveContext();

		//frameRender();
		activeContext->update(deltaTime);
		activeContext->frameRender();

		while (SDL_PollEvent(&event)) {
			done = activeContext->handleEvent(&event);
			switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_F1) {
						if (!isFullscreen) {
							SDL_SetWindowFullscreen(mainwindow, SDL_WINDOW_FULLSCREEN);
							isFullscreen = true;
						}
						else {
							SDL_SetWindowFullscreen(mainwindow, 0);
							isFullscreen = false;
						}
					}
					break;
				case SDL_QUIT:
					done = true;
					break;
				default:
					break;
			}
		}



		// Swap our back buffer to the front
		SDL_GL_SwapWindow(mainwindow);

		SDL_Delay(20); // change this later
	}
}*/

}
