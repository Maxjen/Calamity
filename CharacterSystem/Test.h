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

#include "CharacterEditor3DProperties.h"

#include "FontManager.h"

#include "Gui/WindowManager.h"
#include "Gui/VerticalBoxLayout.h"
#include "Gui/VerticalLayout.h"
#include "Gui/HorizontalLayout.h"
#include "Gui/ContextTabs.h"
#include "Gui/TestWidget.h"
#include "Gui/ButtonGroup.h"
#include "Gui/LineEdit.h"
#include "Gui/Checkbox.h"
#include "Gui/Bone3DItem.h"

#include "ActionHandler/MoveSelection3DHandler.h"
#include "ActionManager.h"
#include "Actions/MoveSelection3D.h"

#include "Functors/LoadBone3DListFunctor.h"

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btDefaultSoftBodySolver.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include "DebugDraw.h"

#include "Camera.h"
#include "Selection3D.h"
#include "MeshObject.h"
#include "ClothObject.h"
#include "CharacterObject.h"
//#include "IKTestObject.h"
#include "Terrain.h"
#include "Lights.h"

using namespace clm;

class Test : public Context {
private:
	/*enum Mode {
		BONE_MODE,
		SKIN_MODE,
	};

	Mode mode;*/

	int screenWidth, screenHeight;

	SpriteShader* spriteShader;
	RadialBlurShader* radialBlurShader;
	FontShader* fontShader;
	ColorShader* colorShader;
	MeshShader* meshShader;
	CharacterMeshShader* characterMeshShader;
	Color3DShader* color3DShader;
	NullShader* nullShader;
	DeferredShader* deferredShader;

	float viewPosX, viewPosY;
	glm::mat4 projectionMatrix;
	//glm::mat4 modelViewMatrix;

	ContextManager* contextManager;

	SpriteManager* spriteManager;

	CharacterEditor3DProperties characterEditor3DProperties;

	ActionManager actionManager;

	unsigned int fontId;
	WindowManager windowManager;
	clm::Window* leftWindow;

	ActionHandler* actionHandler;

	FontManager* fontManager;

	/*btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;*/

	unique_ptr<btBroadphaseInterface> broadphase;
	unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
	unique_ptr<btCollisionDispatcher> dispatcher;
	unique_ptr<btSequentialImpulseConstraintSolver> solver;
	unique_ptr<btSoftBodySolver> softBodySolver;
	//unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
	unique_ptr<btSoftRigidDynamicsWorld> dynamicsWorld;

	DebugDraw debugDraw;

	GBuffer gBuffer;

	//PointLightOld cameraLight;
	unsigned int cameraLightId;
	Camera camera;
	bool bPlayMode;
	Selection3D selection;
	unique_ptr<MeshObject> testMesh;
	unique_ptr<CharacterObject> testCharacter;
	unique_ptr<ClothObject> cloth;
	//unique_ptr<IKTestObject> ikTest;
	unique_ptr<Terrain> terrain;

	Lights lights;

	float deltaTime;
	float runningTime;

	unique_ptr<LoadBone3DListFunctor> loadBone3DListFunctor;
public:
	Test(SpriteShader* spriteShader, RadialBlurShader* radialBlurShader, FontShader* fontShader, ColorShader* colorShader,
		 MeshShader* meshShader,
		 CharacterMeshShader* characterMeshShader, Color3DShader* color3DShader, NullShader* nullShader, DeferredShader* deferredShader,
		 ContextManager* contextManager, SpriteManager* spriteManager, FontManager* fontManager, int screenWidth, int screenHeight);

	void activate();
	void deactivate();

	void initGui();
	void loadBonePanel();
	void loadSkinPanel();

	void selectObject(int mouseX, int mouseY);

	string getContextName();
	bool handleEvent(SDL_Event* event);
	void update(float deltaTime);
	void frameRender();
};
