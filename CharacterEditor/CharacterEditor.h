#pragma once

#include <GL/glew.h>

//#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <dirent.h>

#include "ContextManager.h"
//#include "Context.h"

#include "Shader/SpriteShader.h"
#include "Shader/RadialBlurShader.h"
#include "Shader/FontShader.h"
#include "Shader/ColorShader.h"
#include "SpriteManager.h"
#include "FboManager.h"

#include "../Common/GameManager.h"
#include "../Common/SpriteObject.h"
#include "../Common/BoneObject.h"
//#include "SpriteSelection.h"
#include "BoneSelection.h"
//#include "BoneProperties.h"
#include "CharacterEditorProperties.h"

#include "../Common/BoneManager.h"

#include "FontManager.h"

#include "Gui/WindowManager.h"
#include "Gui/VerticalBoxLayout.h"
#include "Gui/VerticalLayout.h"
#include "Gui/HorizontalLayout.h"
#include "Gui/ContextTabs.h"
#include "Gui/SegmentControl.h"
#include "Gui/TestWidget.h"
#include "Gui/ButtonGroup.h"
#include "Gui/InputGroup.h"
#include "Gui/LineEdit.h"
#include "Gui/Checkbox.h"
#include "../Common/Gui/LayerItem.h"

#include "ActionHandler/CreateBonesHandler.h"
#include "ActionHandler/MoveBonesHandler.h"
#include "ActionHandler/ScaleBonesHandler.h"
#include "ActionHandler/RotateBonesHandler.h"
#include "ActionManager.h"
#include "Actions/RemoveBones.h"
#include "Actions/SelectBones.h"
#include "Actions/SetBonesParent.h"

using namespace clm;

class CharacterEditor : public Context {
private:
	int screenWidth, screenHeight;

	SpriteShader* spriteShader;
	RadialBlurShader* radialBlurShader;
	FontShader* fontShader;
	ColorShader* colorShader;

	float viewPosX, viewPosY;
	/*glm::mat4 projectionMatrix;
	glm::mat4 modelViewMatrix;*/

	ContextManager* contextManager;

	SpriteManager* spriteManager;

	GameManager gameManager;
	BoneManager boneManager;
	unsigned int backLayer;
	unsigned int middleLayer;
	unsigned int frontLayer;

	unsigned int boneLayer;



	//SpriteSelection* spriteSelection;
	BoneSelection boneSelection;
	//BoneProperties boneProperties;

	ActionManager actionManager;

	CharacterEditorProperties characterEditorProperties;

	unique_ptr<RenderLayer> bgLayer;
	unique_ptr<RenderLayer> moonLayer;
	unsigned int bg;
	unsigned int moon;

	unsigned int fontId;
	WindowManager windowManager;
	clm::Window* leftWindow;

	FboManager fboManager;
	unsigned int backgroundLightFbo;

	ActionHandler* actionHandler;

	FontManager* fontManager;
public:
	CharacterEditor(SpriteShader* spriteShader, RadialBlurShader* radialBlurShader, FontShader* fontShader, ColorShader* colorShader,
			  ContextManager* contextManager, SpriteManager* spriteManager, FontManager* fontManager, int screenWidth, int screenHeight);

	//void setMode(Mode mode);

	void initGui();

	string getContextName();
	bool handleEvent(SDL_Event* event);
	void update(float deltaTime);
	void frameRender();
};
