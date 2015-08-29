#pragma once

//#include <GL/glew.h>
//#include <GL/gl.h>

//#define GLM_FORCE_RADIANS
#include <glm/gtx/transform.hpp>

#include <memory>
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
#include "../Common/SharedTriangleData.h"
#include "../Common/TriangleObject.h"
#include "../Common/SpriteObject.h"
#include "MapEditorProperties.h"
#include "MapObjectProperties.h"
#include "../Common/LayerManager.h"
#include "LayerProperties.h"

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
#include "Gui/ObjectItem.h"
#include "../Common/Gui/LayerItem.h"

#include "ActionHandler/PanViewHandler.h"
#include "ActionHandler/CreateTriangleHandler.h"
#include "ActionHandler/MoveObjectsHandler.h"
#include "ActionHandler/ScaleObjectsHandler.h"
#include "ActionHandler/RotateObjectsHandler.h"
#include "ActionManager.h"
#include "Actions/FillTriangle.h"
#include "Actions/CreateSpriteObject.h"
#include "Actions/RemoveObjects.h"
#include "Actions/SelectObjects.h"
#include "Actions/CreateLayer.h"

//#include "Functor.h"
#include "Functors/CreateLayerFunctor.h"
#include "Functors/RemoveLayerFunctor.h"
#include "Functors/MoveLayerFunctor.h"
#include "Functors/SelectLayerFunctor.h"
#include "Functors/LoadLayerListFunctor.h"

using namespace clm;

class MapEditor : public Context {
private:
	int screenWidth, screenHeight;

	SpriteShader* spriteShader;
	TextureShader* textureShader;
	RadialBlurShader* radialBlurShader;
	FontShader* fontShader;
	ColorShader* colorShader;

	float viewPosX, viewPosY;
	/*glm::mat4 projectionMatrix;
	glm::mat4 modelViewMatrix;*/

	ContextManager* contextManager;

	SpriteManager* spriteManager;

	GameManager gameManager;
	SharedTriangleData sharedTriangleData;

	LayerManager layerManager;
	LayerProperties layerProperties;

	MapEditorSelection mapEditorSelection;

	MapEditorProperties mapEditorProperties;

	MapObjectProperties mapObjectProperties;

	ActionManager actionManager;

	/*RenderLayer* bgLayer;
	RenderLayer* moonLayer;*/
	unique_ptr<RenderLayer> bgLayer;
	unique_ptr<RenderLayer> moonLayer;
	//RenderLayer* textLayer;
	unsigned int bg;
	unsigned int moon;

	unsigned int fontId;
	WindowManager windowManager;

	//FboManager* fboManager;
	//unique_ptr<FboManager> fboManager;
	FboManager fboManager;
	unsigned int backgroundLightFbo;
	unsigned int backgroundLightFbo2;
	unsigned int lightFbo;
	unsigned int lightLayer;

	ActionHandler* actionHandler;

	FontManager* fontManager;
	/*unsigned int text1;
	unsigned int text2;
	unsigned int text3;

	unsigned int font1;
	unsigned int font2;
	unsigned int font3;*/

	bool leftColorMask[3];
	bool rightColorMask[3];
	bool combinedColorMask[3];

	unique_ptr<CreateLayerFunctor> createLayerFunctor;
	unique_ptr<RemoveLayerFunctor> removeLayerFunctor;
	unique_ptr<MoveLayerFunctor> moveLayerUpFunctor;
	unique_ptr<MoveLayerFunctor> moveLayerDownFunctor;
	unique_ptr<SelectLayerFunctor> selectLayerFunctor;
	unique_ptr<LoadLayerListFunctor> loadLayerListFunctor;

	//unsigned int line1;
public:
	MapEditor(SpriteShader* spriteShader, TextureShader* textureShader, RadialBlurShader* radialBlurShader,
			  FontShader* fontShader, ColorShader* colorShader, ContextManager* contextManager,
			  SpriteManager* spriteManager, FontManager* fontManager, int screenWidth, int screenHeight);

	void initGui();

	string getContextName();
	bool handleEvent(SDL_Event* event);
	void update(float deltaTime);
	void frameRender();
};
