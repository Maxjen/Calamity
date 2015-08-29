//#include <iostream>
#include <memory>
#include <cstdio>
#include "Calamity.h"

#include "MapEditor/MapEditor.h"
#include "CharacterEditor/CharacterEditor.h"
#include "CharacterSystem/Test.h"

using std::unique_ptr;

using namespace clm;

int main() {
	Core* core = new Core();
	//std::unique_ptr<Core> core(new Core());
	//if (core->init(800, 600, "Game") < 0) {
	/*int screenWidth = 1366;
	int screenHeight = 768;*/
	if (core->init("Game") < 0) {
		printf("Unable to initialize Calamity Core\n");
		//core.reset();
		delete core;
		return -1;
	}
	else {
		int screenWidth = core->getScreenWidth();
		int screenHeight = core->getScreenHeight();

		TextureManager textureManager;
		FontManager fontManager;
		SpriteShader spriteShader;
		TextureShader textureShader;
		FontShader fontShader;
		ColorShader colorShader;
		RadialBlurShader radialBlurShader(screenWidth, screenHeight);
		MeshShader meshShader;
		CharacterMeshShader characterMeshShader;
		Color3DShader color3DShader;
		NullShader nullShader;
		DeferredShader deferredShader;
		Renderer renderer(&textureManager, &fontManager, &spriteShader, &textureShader, &fontShader, &colorShader);
		SpriteManager spriteManager(&renderer, screenWidth, screenHeight);

		MapEditor* mapEditor = new MapEditor(&spriteShader, &textureShader, &radialBlurShader, &fontShader, &colorShader,
											 core->getContextManager(), &spriteManager, &fontManager, screenWidth, screenHeight);
		unsigned int mapEditorId = core->addContext(mapEditor);

		CharacterEditor* characterEditor = new CharacterEditor(&spriteShader, &radialBlurShader, &fontShader, &colorShader,
											 core->getContextManager(), &spriteManager, &fontManager, screenWidth, screenHeight);
		core->addContext(characterEditor);

		Test* test = new Test(&spriteShader, &radialBlurShader, &fontShader, &colorShader,
							  &meshShader,
							  &characterMeshShader, &color3DShader, &nullShader, &deferredShader,
							  core->getContextManager(), &spriteManager, &fontManager, screenWidth, screenHeight);
		unsigned int testId = core->addContext(test);

		core->setCurrentContext(testId);

		mapEditor->initGui();
		characterEditor->initGui();
		test->initGui();

		core->mainLoop();

		//delete mapEditor;
		//delete characterEditor;
		delete core;
		//core.reset();

		return 0;
	}
	//return 0;
}

