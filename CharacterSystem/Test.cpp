#include "Test.h"

Test::Test(SpriteShader* spriteShader, RadialBlurShader* radialBlurShader, FontShader* fontShader, ColorShader* colorShader,
		   MeshShader* meshShader,
		   CharacterMeshShader* characterMeshShader, Color3DShader* color3DShader, NullShader* nullShader,
		   DeferredShader* deferredShader, ContextManager* contextManager, SpriteManager* spriteManager, FontManager* fontManager,
		   int screenWidth, int screenHeight) :
	characterEditor3DProperties(spriteManager),
	windowManager(spriteManager),
	debugDraw(color3DShader),
	camera(screenWidth, screenHeight),
	lights(&camera, &gBuffer, nullShader, deferredShader)
{
	this->spriteShader = spriteShader;
	this->radialBlurShader = radialBlurShader;
	this->fontShader = fontShader;
	this->colorShader = colorShader;
	this->meshShader = meshShader;
	this->characterMeshShader = characterMeshShader;
	this->color3DShader = color3DShader;
	this->nullShader = nullShader;
	this->deferredShader = deferredShader;
	this->contextManager = contextManager;
	this->spriteManager = spriteManager;
	this->fontManager = fontManager;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	glm::mat4 projectionMatrixTmp = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -128.0f, 128.0f);
	glm::mat4 modelViewMatrix = glm::mat4();
	//glm::mat4 modelViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	viewPosX = viewPosY = 0.0f;


	//spriteShader.loadFromFiles("Calamity/Shader/sprite.vert", "Calamity/Shader/sprite.frag");

	spriteShader->bind();
	spriteShader->setProjectionMatrix(projectionMatrixTmp);
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();

	radialBlurShader->bind();
	radialBlurShader->setProjectionMatrix(projectionMatrixTmp);
	radialBlurShader->setModelViewMatrix(modelViewMatrix);
	radialBlurShader->unbind();

	fontShader->bind();
	fontShader->setProjectionMatrix(projectionMatrixTmp);
	fontShader->setModelViewMatrix(modelViewMatrix);
	fontShader->unbind();

	colorShader->bind();
	colorShader->setProjectionMatrix(projectionMatrixTmp);
	colorShader->setModelViewMatrix(modelViewMatrix);
	colorShader->unbind();

	/*projectionMatrix = glm::perspective(
		45.0f,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		((float)screenWidth) / screenHeight, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		100.0f       // Far clipping plane. Keep as little as possible.
	);*/

	projectionMatrix = glm::perspective(
			45.0f,         // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
			((float)screenWidth) / screenHeight, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
			0.1f,        // Near clipping plane. Keep as big as possible, or you'll get precision issues.
			300.0f       // Far clipping plane. Keep as little as possible.
		);

	/*modelViewMatrix = glm::lookAt(
		glm::vec3(8,6,-6), // Camera is at (4,3,3), in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);*/

	modelViewMatrix = camera.getViewMatrix();

	//modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(0.06f, 0.06f, 0.06f));

	meshShader->bind();
	meshShader->setProjectionMatrix(projectionMatrix);
	meshShader->setModelViewMatrix(modelViewMatrix);
	//meshShader->setDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f), 0.1f, 0.8f);
	meshShader->unbind();

	/*cameraLight.color = glm::vec3(0, 1, 0);
	cameraLight.ambientIntensity = 0.1f;
	cameraLight.diffuseIntensity = 0.8f;
	cameraLight.position = glm::vec3(0, 0, 0);
	cameraLight.attConstant = 1;
	cameraLight.attLinear = 0;
	cameraLight.attExponential = 1;

	PointLightOld pointLight1;
	pointLight1.color = glm::vec3(0, 0, 1);
	pointLight1.ambientIntensity = 0.1f;
	pointLight1.diffuseIntensity = 0.8f;
	pointLight1.position = glm::vec3(0, 2, 0);
	pointLight1.attConstant = 1;
	pointLight1.attLinear = 0;
	pointLight1.attExponential = 1;

	characterMeshShader->bind();
	characterMeshShader->setProjectionMatrix(projectionMatrix);
	characterMeshShader->setModelViewMatrix(modelViewMatrix);
	characterMeshShader->setDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f), 0.1f, 0.8f);
	cameraLightId = characterMeshShader->addPointLight(cameraLight);
	//characterMeshShader->addPointLight(pointLight1);
	characterMeshShader->unbind();*/

	characterMeshShader->bind();
	characterMeshShader->setProjectionMatrix(projectionMatrix);
	characterMeshShader->setModelViewMatrix(modelViewMatrix);
	characterMeshShader->unbind();

	color3DShader->bind();
	color3DShader->setProjectionMatrix(projectionMatrix);
	color3DShader->setModelViewMatrix(modelViewMatrix);
	color3DShader->unbind();

	nullShader->bind();
	nullShader->setProjectionMatrix(projectionMatrix);
	nullShader->setModelViewMatrix(modelViewMatrix);
	nullShader->unbind();

	deferredShader->bind();
	deferredShader->setProjectionMatrix(projectionMatrix);
	deferredShader->setModelViewMatrix(modelViewMatrix);
	deferredShader->setScreenSize(screenWidth, screenHeight);
	deferredShader->unbind();

	/*broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	dynamicsWorld->setDebugDrawer(&debugDraw);*/

	broadphase.reset(new btDbvtBroadphase());
	//collisionConfiguration.reset(new btDefaultCollisionConfiguration());
	collisionConfiguration.reset(new btSoftBodyRigidBodyCollisionConfiguration());
	dispatcher.reset(new btCollisionDispatcher(collisionConfiguration.get()));
	solver.reset(new btSequentialImpulseConstraintSolver);
	softBodySolver.reset(new btDefaultSoftBodySolver);
	//dynamicsWorld.reset(new btDiscreteDynamicsWorld(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get()));
	dynamicsWorld.reset(new btSoftRigidDynamicsWorld(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get(), softBodySolver.get()));
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	dynamicsWorld->setDebugDrawer(&debugDraw);
	debugDraw.setDebugMode(0);

	/*btSoftBodyWorldInfo softBodyWorldInfo;
	softBodyWorldInfo.air_density = (btScalar)1.2;
	softBodyWorldInfo.water_density = 0;
	softBodyWorldInfo.water_offset = 0;
	softBodyWorldInfo.water_normal = btVector3(0, 0, 0);
	softBodyWorldInfo.m_gravity.setValue(0, -10, 0);*/

	gBuffer.init(screenWidth, screenHeight);

	camera.setPosition(0, 0.2f, 3);

	bPlayMode = false;

	/*testMesh.reset(new MeshObject(&camera, meshShader, dynamicsWorld.get()));
	//testMesh->loadMesh("Data/phoenix_ugv.md2");
	//testMesh->setScale(0.06f, 0.06f, 0.06f);
	//testMesh->loadMesh("Data/file.obj");
	//testMesh->loadMesh("Data/h01.fbx");
	//testMesh->loadMesh("Data/cloth.fbx");
	testMesh->loadMesh("Data/monkey.fbx");
	//testMesh->loadMesh("Data/testCharacter.fbx");
	//testMesh->setPosition(-3.0f, 1.0f, 0);
	//testMesh->setScale(0.1f, 0.1f, 0.1f);
	//testMesh->loadMesh("Data/face1_02.fbx");
	//testMesh->loadMesh("Data/face2_04.fbx");*/

	testMesh.reset(new MeshObject(&camera, meshShader, dynamicsWorld.get()));
	testMesh->loadMesh("Data/cube.fbx");
	testMesh->setPosition(6.0f, 0, 0);

	testCharacter.reset(new CharacterObject(&camera, characterMeshShader, meshShader, dynamicsWorld.get(), &debugDraw, &characterEditor3DProperties));
	//testCharacter->loadSkeleton("Data/characterBody_new3.fbx");
	//testCharacter->loadMesh("Data/characterBody_new3.fbx");
	testCharacter->loadSkeleton("Data/Skeleton/characterBaseSkeleton.fbx");
	testCharacter->addMeshComponent("Data/MeshComponents/characterBase.fbx");
	testCharacter->createRagdoll();
	testCharacter->addClothComponent("Data/ClothComponents/cape.fbx");

	//testCharacter->loadMesh("Data/characterBaseM_c01.fbx");


	/*testCharacter->loadSkeleton("Data/characterBody_normalized2.fbx");
	testCharacter->loadMesh("Data/characterBody_normalized2.fbx");*/
	/*testCharacter->loadSkeleton("Data/testSkeleton.fbx");
	testCharacter->loadMesh("Data/testSkeleton.fbx");*/
	//testCharacter->loadMesh("Data/characterHelmet.fbx");
	//testCharacter->setPosition(0, -1, 0);
	/*testCharacter->loadMesh("Data/boblampclean.md5mesh");
	testCharacter->setScale(0.1f, 0.1f, 0.1f);*/
	//testCharacter->loadMesh("Data/monkey.fbx");
	//testCharacter->loadMesh("Data/testArmature.fbx");

	//testCharacter->update(0);

	cloth.reset(nullptr);
	/*cloth.reset(new ClothObject(&camera, meshShader, dynamicsWorld.get()));
	cloth->setPosition(0, 4.0f, 0);
	//cloth->loadMesh("Data/cape4rotated.fbx");
	cloth->loadMesh("Data/characterBaseM_cape3.fbx");
	cloth->attachToRigidBody(testCharacter->getChestRigidBody());*/

	/*ikTest.reset(new IKTestObject(&debugDraw));
	ikTest->setPosition(-3.0f, 0, 0);*/

	//camera.setThirdPerson(controller->getRigidBody());
	//camera.setThirdPerson(testCharacter->getControllerRigidBody());
	camera.setThirdPerson(testCharacter->getCharacterController()->getControllerRigidBody());

	terrain.reset(new Terrain(&camera, meshShader, dynamicsWorld.get()));
	terrain->loadTerrain();


	PointLight p1;
	p1.color = glm::vec3(0, 0, 1);
	p1.ambientIntensity = 0.1f;
	p1.diffuseIntensity = 10.0f;
	p1.position = glm::vec3(0, 5, 0);
	p1.attConstant = 1;
	p1.attLinear = 0;
	p1.attExponential = 1;

	PointLight p2;
	p2.color = glm::vec3(1, 0, 0);
	p2.ambientIntensity = 0.1f;
	p2.diffuseIntensity = 0.3f;
	p2.position = glm::vec3(0, -2, 0);
	p2.attConstant = 1;
	p2.attLinear = 0;
	p2.attExponential = 1;

	PointLight p3;
	p3.color = glm::vec3(1, 1, 1);
	p3.ambientIntensity = 0.1f;
	p3.diffuseIntensity = 70.0f;
	p3.position = glm::vec3(0, 15, 5);
	p3.attConstant = 1;
	p3.attLinear = 0;
	p3.attExponential = 1;

	lights.loadMesh();
	lights.addPointLight(p1);
	lights.addPointLight(p2);
	lights.addPointLight(p3);


	characterEditor3DProperties.setCharacter(testCharacter.get());
	characterEditor3DProperties.setDebugDraw(&debugDraw);

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("OpenGL %d.%d\n", major, minor);


	actionHandler = nullptr;



	//initGui();

	fontId = fontManager->createFont("Fonts/DejaVuSans.ttf", 10);

	characterEditor3DProperties.setFontId(fontId);

	//fontManager->writeToPng(fontId);

	deltaTime = 0;
	runningTime = 0;

	loadBone3DListFunctor.reset(new LoadBone3DListFunctor(spriteManager, fontId, testCharacter.get()));


	//spriteManager->uploadTextures();
	//renderer->writeTextureAtlasesToPng();
}

void Test::activate() {
	// Enable Z-buffer read and write
	/*glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE); // enabled by default
	glDepthFunc(GL_LEQUAL);*/

	/*glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);*/
}

void Test::deactivate() {
	//glDisable(GL_DEPTH_TEST);
	/*glDisable(GL_CULL_FACE);*/
	glLineWidth(1);
}

/*void CharacterEditor::loadBonePanel() {
	leftWindow->clear();

	VerticalBoxLayout* leftBoxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	leftWindow->setChild(leftBoxLayout);

	VerticalLayout* bonesLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int boneListBox = leftBoxLayout->addBox();
	leftBoxLayout->setBoxWidget(boneListBox, bonesLayout);

	//ListView* boneList = new ListView(spriteManager, 100, 100, true, true);
	boneList = new ListView(spriteManager, 100, 100, true, true);
	bonesLayout->addChild(boneList);

	loadBoneListFunctor.reset(new LoadBoneListFunctor(boneList, &boneManager, &boneSelection, &actionManager, spriteManager, fontId));
	boneList->setLoadFunctor(loadBoneListFunctor.get());
	boneList->reload();

	ButtonGroup* boneButtons = new ButtonGroup(spriteManager, fontId, 150, true);
	bonesLayout->addChild(boneButtons);
	boneButtons->addRow();
	boneButtons->addButton(0, L"Move up");
	boneButtons->addButton(0, L"Move down");

	VerticalLayout* skinsLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int skinListBox = leftBoxLayout->addBox();
	leftBoxLayout->setBoxWidget(skinListBox, skinsLayout);

	ListView* skinList = new ListView(spriteManager, 100, 100, true, true);
	skinsLayout->addChild(skinList);

	ButtonGroup* skinButtons = new ButtonGroup(spriteManager, fontId, 150, true);
	skinsLayout->addChild(skinButtons);
	skinButtons->addRow();
	skinButtons->addButton(0, L"Add skin");
	skinButtons->addButton(0, L"Remove skin");
}*/

void Test::initGui() {
	unsigned int topWindowId = windowManager.createWindow(0, 0, screenWidth, 50, CLM_BOTTOM_LEVEL | CLM_NO_MOVE);
	Window* topWindow = windowManager.getWindow(topWindowId);
	topWindow->setPadding(0);

	VerticalLayout* topPanelLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	topWindow->setChild(topPanelLayout);

	ContextTabs* contextTabs = new ContextTabs(spriteManager, contextManager, contextId, screenWidth, true);
	{
		list<unsigned int> contextIds = contextManager->getContextIds();
		for (auto& it : contextIds) {
			bool isActive = it == contextId;
			string contextName = contextManager->getContextName(it);
			wstring label(contextName.begin(), contextName.end());
			ContextTab* tab = new ContextTab(spriteManager, it, isActive, label, fontId);
			contextTabs->addTab(tab);
		}
	}

	topPanelLayout->addChild(contextTabs);

	HorizontalLayout* menuLayout = new HorizontalLayout(spriteManager, 0, 0, true, true);
	menuLayout->setMargin(6, 6, 0, 6);
	topPanelLayout->addChild(menuLayout);
	/*Checkbox* checkTest = new Checkbox(spriteManager, 100, L"File  Edit  Options", fontId, nullptr, nullptr, true);
	menuLayout->addChild(checkTest);*/
	ButtonGroup* editorModeButtons1 = new ButtonGroup(spriteManager, fontId, 200);
	menuLayout->addChild(editorModeButtons1);
	editorModeButtons1->addRow();
	editorModeButtons1->addButton(0, L"Save", nullptr);
	editorModeButtons1->addButton(0, L"Load", nullptr);
	editorModeButtons1->addButton(0, L"Options", nullptr);
	/*InputGroup* eyeOffsetInput = new InputGroup(spriteManager, fontId, 150);
	menuLayout->addChild(eyeOffsetInput);
	eyeOffsetInput->addRow();
	eyeOffsetInput->addInput(0, L"EyeOffset", &mapEditorProperties, mapEditorProperties.getEyeOffsetProperty());*/
	Checkbox* checkShowGBuffer = new Checkbox(spriteManager, 100, L"show GBuffer", fontId, &characterEditor3DProperties, characterEditor3DProperties.getShowGBufferProperty(), true);
	menuLayout->addChild(checkShowGBuffer);
	SegmentControl* characterEditor3DModeSC = new SegmentControl(spriteManager, fontId, 150, &characterEditor3DProperties, characterEditor3DProperties.getCharacterEditor3DModeProperty());
	menuLayout->addChild(characterEditor3DModeSC);
	characterEditor3DModeSC->addRow();
	characterEditor3DModeSC->addSegment(0, L"Character", CHARACTER_MODE);
	characterEditor3DModeSC->addSegment(0, L"Physics", PHYSICS_MODE);


	unsigned int leftWindowId = windowManager.createWindow(0, 57, 180, screenHeight - 57, CLM_BOTTOM_LEVEL | CLM_RESIZE_RIGHT | CLM_NO_MOVE);
	leftWindow = windowManager.getWindow(leftWindowId);
	leftWindow->setPadding(0);

	characterEditor3DProperties.setWindow(leftWindow);
	characterEditor3DProperties.loadCharacterPanel();

	/*VerticalBoxLayout* leftBoxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	leftWindow->setChild(leftBoxLayout);

	VerticalLayout* viewOptionsLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int viewOptionsBox = leftBoxLayout->addBox();
	leftBoxLayout->setBoxWidget(viewOptionsBox, viewOptionsLayout);

	Checkbox* checkShowBones = new Checkbox(spriteManager, 100, L"show bones", fontId, &characterEditor3DProperties, characterEditor3DProperties.getShowBonesProperty(), true);
	viewOptionsLayout->addChild(checkShowBones);

	Checkbox* checkShowRigidBodies = new Checkbox(spriteManager, 100, L"show rigid bodies", fontId, &characterEditor3DProperties, characterEditor3DProperties.getShowRigidBodiesProperty(), true);
	viewOptionsLayout->addChild(checkShowRigidBodies);

	Checkbox* checkShowConstraints = new Checkbox(spriteManager, 100, L"show constraints", fontId, &characterEditor3DProperties, characterEditor3DProperties.getShowConstraintsProperty(), true);
	viewOptionsLayout->addChild(checkShowConstraints);

	Checkbox* checkShowGBuffer = new Checkbox(spriteManager, 100, L"show GBuffer", fontId, &characterEditor3DProperties, characterEditor3DProperties.getShowGBufferProperty(), true);
	viewOptionsLayout->addChild(checkShowGBuffer);*/


	unsigned int rightWindowId = windowManager.createWindow(screenWidth - 180, 57, 180, screenHeight - 57, CLM_BOTTOM_LEVEL | CLM_RESIZE_LEFT | CLM_NO_MOVE);
	Window* rightWindow = windowManager.getWindow(rightWindowId);
	rightWindow->setPadding(0);

	VerticalBoxLayout* rightBoxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	rightWindow->setChild(rightBoxLayout);

	VerticalLayout* layersLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int  layerListBox = rightBoxLayout->addBox();
	rightBoxLayout->setBoxWidget(layerListBox, layersLayout);

	TreeList* boneList = new TreeList(spriteManager, 100, 100, true, true);
	layersLayout->addChild(boneList);

	loadBone3DListFunctor->setBoneList(boneList);
	boneList->setLoadFunctor(loadBone3DListFunctor.get());
	boneList->reload();

	/*ButtonGroup* layerButtons = new ButtonGroup(spriteManager, fontId, 150, true);
	layersLayout->addChild(layerButtons);
	layerButtons->addRow();
	layerButtons->addButton(0, L"Move up");
	layerButtons->addButton(0, L"Move down");
	layerButtons->addRow();
	layerButtons->addButton(1, L"Add layer");
	layerButtons->addButton(1, L"Remove layer");*/

	/*LineEdit* layerNameEdit = new LineEdit(spriteManager, 100, L"Layer", fontId, true);
	layersLayout->addChild(layerNameEdit);*/


	spriteManager->uploadTextures();
}

void Test::selectObject(int mouseX, int mouseY) {
	glm::vec4 rayStartNDC(
		((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
		((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
		-1.0, // The near plane maps to z = -1 in Normalized Device Coordinates
		1.0f
	);
	glm::vec4 rayEndNDC(
		((float)mouseX/(float)screenWidth  - 0.5f) * 2.0f,
		((float)mouseY/(float)screenHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
	);

	glm::mat4 M = glm::inverse(projectionMatrix * camera.getViewMatrix());
	glm::vec4 rayStartWorld = M * rayStartNDC;
	rayStartWorld /= rayStartWorld.w;
	glm::vec4 rayEndWorld = M * rayEndNDC;
	rayEndWorld /= rayEndWorld.w;

	glm::vec3 rayDirectionWorld(rayEndWorld - rayStartWorld);
	rayDirectionWorld = glm::normalize(rayDirectionWorld);

	rayDirectionWorld = rayDirectionWorld * 1000.0f;

	btVector3 start(rayStartWorld.x, rayStartWorld.y, rayStartWorld.z);
	btVector3 direction(rayDirectionWorld.x, rayDirectionWorld.y, rayDirectionWorld.z);

	btCollisionWorld::ClosestRayResultCallback rayCallback(start, direction);
	dynamicsWorld->rayTest(start, direction, rayCallback);

	if (rayCallback.hasHit()) {
		//printf("hit!\n");
		GameObject3D* object = (GameObject3D*)rayCallback.m_collisionObject->getUserPointer();
		if (object && object->getObjectType() == MESH_OBJECT) {
			((MeshObject*)object)->toggleSelection();
			selection.selectMeshObject((MeshObject*)object);
		}
	}
	/*else {
		printf("hit nothing\n");
	}*/

	/*direction.normalize();
	direction *= 10;
	direction += start;
	testMesh->setPosition(start.getX(), start.getY(), start.getZ());
	testMesh2->setPosition(direction.getX(), direction.getY(), direction.getZ());*/
}

string Test::getContextName() {
	string result = "3D Character System";
	return result;
}

bool Test::handleEvent(SDL_Event* event) {
	float moveSpeed = 20.0f;

	if (actionHandler != nullptr) {
		if (actionHandler->handleEvent(event)) {
			delete actionHandler;
			actionHandler = nullptr;
		}
	}
	else {
		switch (event->type) {
			case SDL_MOUSEMOTION: {
				const Uint8* keyState = SDL_GetKeyboardState(NULL);
				float speed = 0.1f;

				if (keyState[SDL_SCANCODE_LSHIFT])
					camera.changeAngles(-event->motion.xrel * deltaTime * speed, -event->motion.yrel * deltaTime * speed);
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
				if (event->button.button == SDL_BUTTON_LEFT) {
					if (windowManager.contains(event->button.x, screenHeight - event->button.y)) {
						actionHandler = windowManager.createActionHandler(event->button.x, screenHeight - event->button.y, event);
					}
					/*else {
						float scrollSpeed = gameManager.getLayerScrollSpeed(boneLayer);
						float x = viewPosX * scrollSpeed + event->button.x;
						float y = viewPosY * scrollSpeed + screenHeight - event->button.y;

						list<Vec2> startingPositions;
						startingPositions.push_back(Vec2(x, y));
						actionHandler = new CreateBonesHandler(&boneManager, &boneSelection, boneList, &actionManager, spriteManager, startingPositions, boneLayer);
					}*/
				}
				/*else if (event->button.button == SDL_BUTTON_MIDDLE) {
					actionHandler = new PanViewHandler(&viewPosX, &viewPosY);
				}*/
				else if (event->button.button == SDL_BUTTON_RIGHT) {
					if (windowManager.contains(event->button.x, screenHeight - event->button.y)) {
						actionHandler = windowManager.createActionHandler(event->button.x, screenHeight - event->button.y, event);
					}
					else {
						selectObject(event->button.x, screenHeight - event->button.y);
					}
					/*float scrollSpeed = gameManager.getLayerScrollSpeed(boneLayer);
					float x = viewPosX * scrollSpeed + event->button.x;
					float y = viewPosY * scrollSpeed + screenHeight - event->button.y;
					list<unsigned int> possibleObjects = gameManager.getObjectsAtPosition(boneLayer, x, y);
					unordered_map<unsigned int, int> newBones;
					if (!possibleObjects.empty()) {
						float minDist = 100;
						for (auto& it : possibleObjects) {
							BoneVertexDistance c = ((BoneObject*)(gameManager.getObject(it)))->getClosestBoneVertex(x, y);
							if (c.distance == minDist)
								newBones[it] = c.boneVertex;
							else if (c.distance < minDist) {
								newBones.clear();
								newBones[it] = c.boneVertex;
								minDist = c.distance;
							}
						}
					}
					if (!newBones.empty()) {
						const Uint8* keyState = SDL_GetKeyboardState(NULL);
						SelectBones* action;
						if (keyState[SDL_SCANCODE_LSHIFT])
							action = new SelectBones(&boneSelection, newBones);
						else
							action = new SelectBones(&boneSelection, newBones, true);
						actionManager.pushAction(action);
					}*/
				}
				break;
			case SDL_MOUSEWHEEL:
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				y = screenHeight - y;
				if (windowManager.contains(x, y)) {
					actionHandler = windowManager.createActionHandler(x, y, event);
				}
				break;
			}
			case SDL_KEYDOWN:
				switch (event->key.keysym.sym) {
					case SDLK_TAB:
					{
						IntProperty* characterEditor3DModeProperty = characterEditor3DProperties.getCharacterEditor3DModeProperty();
						wstring newCharacterEditor3DMode = characterEditor3DProperties.getPreviousCharacterEditor3DMode();
						wstring oldCharacterEditor3DMode = characterEditor3DModeProperty->getValueAsString();
						characterEditor3DProperties.pushPropertyChange(characterEditor3DModeProperty, oldCharacterEditor3DMode, newCharacterEditor3DMode);
						break;
					}
					case SDLK_LSHIFT:
						SDL_SetRelativeMouseMode(SDL_TRUE);
						break;
					case SDLK_LEFT:
					case SDLK_a: {
						if (bPlayMode) {
							//controller->setWalkingX(-1);
							testCharacter->getCharacterController()->setWalkingX(-1);
						}
						else {
							const Uint8* keyState = SDL_GetKeyboardState(NULL);
							if (keyState[SDL_SCANCODE_LSHIFT])
								camera.move(0, -deltaTime * moveSpeed);
						}
						break;
					}
					case SDLK_RIGHT:
					case SDLK_d: {
						if (bPlayMode) {
							//controller->setWalkingX(1);
							testCharacter->getCharacterController()->setWalkingX(1);
						}
						else {
							const Uint8* keyState = SDL_GetKeyboardState(NULL);
							if (keyState[SDL_SCANCODE_LSHIFT])
								camera.move(0, deltaTime * moveSpeed);
						}
						break;
					}
					/*case SDLK_i: {
						ikTest->ikStep();
						break;
					}*/
					case SDLK_UP:
					case SDLK_w: {
						if (bPlayMode) {
							//controller->setWalkingZ(1);
							testCharacter->getCharacterController()->setWalkingZ(1);
						}
						else {
							const Uint8* keyState = SDL_GetKeyboardState(NULL);
							if (keyState[SDL_SCANCODE_LSHIFT])
								camera.move(deltaTime * moveSpeed, 0);
						}
						break;
					}
					case SDLK_DOWN:
					case SDLK_s: {
						if (bPlayMode) {
							//controller->setWalkingZ(-1);
							testCharacter->getCharacterController()->setWalkingZ(-1);
						}
						else {
							const Uint8* keyState = SDL_GetKeyboardState(NULL);
							if (keyState[SDL_SCANCODE_LSHIFT])
								camera.move(-deltaTime * moveSpeed, 0);
						}
						break;
					}
					case SDLK_p: {
						bPlayMode = !bPlayMode;
						if (!bPlayMode) {
							/*controller->setWalkingX(0);
							controller->setWalkingZ(0);*/

							testCharacter->getCharacterController()->setWalkingX(0);
							testCharacter->getCharacterController()->setWalkingZ(0);
						}
						break;
					}
					case SDLK_SPACE:
						testCharacter->setRagdollMode(true);
						break;
					/*case SDLK_a:
					{
						int x, y;
						SDL_GetMouseState(&x, &y);
						y = screenHeight - y;
						if (windowManager.contains(x, y)) {
							actionHandler = windowManager.createActionHandler(x, y, event);
						}
						else {
							unordered_map<unsigned int, int> selectedBones = boneSelection.getSelectedBones();
							if (!selectedBones.empty()) {
								SelectBones* action = new SelectBones(&boneSelection, selectedBones);
								actionManager.pushAction(action);
							}
						}
						break;
					}*/
					case SDLK_g:
					{
						list<MeshObject*> selectedMeshObjects = selection.getSelectedMeshObjects();
						if (!selectedMeshObjects.empty())
							actionHandler = new MoveSelection3DHandler(&selection, &camera, &actionManager);
						break;
					}
					/*case SDLK_r:
					{
						if (boneSelection.moreThanOneVertex()) {
							float scrollSpeed = gameManager.getLayerScrollSpeed(boneLayer);
							int x, y;
							SDL_GetMouseState(&x, &y);
							float mouseX = viewPosX * scrollSpeed + x;
							float mouseY = viewPosY * scrollSpeed + screenHeight - y;
							actionHandler = new RotateBonesHandler(&boneSelection, &actionManager, mouseX, mouseY);
						}
						break;
					}
					case SDLK_s:
					{
						if (boneSelection.moreThanOneVertex()) {
							float scrollSpeed = gameManager.getLayerScrollSpeed(boneLayer);
							int x, y;
							SDL_GetMouseState(&x, &y);
							float mouseX = viewPosX * scrollSpeed + x;
							float mouseY = viewPosY * scrollSpeed + screenHeight - y;
							actionHandler = new ScaleBonesHandler(&boneSelection, &actionManager, mouseX, mouseY);
						}
						break;
					}
					case SDLK_x:
					{
						if (!boneSelection.getSelectedBones().empty()) {
							RemoveBones* action = new RemoveBones(&boneManager, &boneSelection, boneList);
							actionManager.pushAction(action);
						}
					}*/
					case SDLK_z:
					{
						const Uint8* keyState = SDL_GetKeyboardState(NULL);
						if (keyState[SDL_SCANCODE_LCTRL]) {
							if (keyState[SDL_SCANCODE_LSHIFT])
								actionManager.redoAction();
							else
								actionManager.undoAction();
						}
						break;
					}
				}
				break;
			case SDL_KEYUP:
				switch (event->key.keysym.sym) {
					case SDLK_LSHIFT:
						SDL_SetRelativeMouseMode(SDL_FALSE);
						break;
					case SDLK_LEFT:
					case SDLK_a: {
						/*if (bPlayMode && controller->getWalkingX() == -1) {
							controller->setWalkingX(0);
						}*/
						if (bPlayMode && testCharacter->getCharacterController()->getWalkingX() == -1) {
							testCharacter->getCharacterController()->setWalkingX(0);
						}
						break;
					}
					case SDLK_RIGHT:
					case SDLK_d: {
						/*if (bPlayMode && controller->getWalkingX() == 1) {
							controller->setWalkingX(0);
						}*/
						if (bPlayMode && testCharacter->getCharacterController()->getWalkingX() == 1) {
							testCharacter->getCharacterController()->setWalkingX(0);
						}
						break;
					}
					case SDLK_UP:
					case SDLK_w: {
						/*if (bPlayMode && controller->getWalkingZ() == 1) {
							controller->setWalkingZ(0);
						}*/
						if (bPlayMode && testCharacter->getCharacterController()->getWalkingZ() == 1) {
							testCharacter->getCharacterController()->setWalkingZ(0);
						}
						break;
					}
					case SDLK_DOWN:
					case SDLK_s: {
						/*if (bPlayMode && controller->getWalkingZ() == -1) {
							controller->setWalkingZ(0);
						}*/
						if (bPlayMode && testCharacter->getCharacterController()->getWalkingZ() == -1) {
							testCharacter->getCharacterController()->setWalkingZ(0);
						}
						break;
					}
				}
		}
	}

	return false;
}

void Test::update(float deltaTime) {
	//deltaTime /= 10;
	this->deltaTime = deltaTime;
	runningTime += deltaTime;

	dynamicsWorld->stepSimulation(deltaTime, 7);

	//testCharacter->update(runningTime);
	testCharacter->update(deltaTime);
	//cloth->setAPosition(face2->getPosition() + glm::vec3(0, 5, 0));

	if (cloth) {
		btRigidBody* chestRigidBody = testCharacter->getChestRigidBody();
		btTransform transform = chestRigidBody->getCenterOfMassTransform();
		glm::mat4 matrix;
		transform.getOpenGLMatrix(glm::value_ptr(matrix));
		cloth->applyMatrix(matrix);

		cloth->update();
	}

	if (bPlayMode)
		camera.update();

	/*cameraLight.position = camera.getPosition();
	characterMeshShader->bind();
	characterMeshShader->setPointLight(cameraLightId, cameraLight);
	characterMeshShader->unbind();*/
}

void Test::frameRender() {
	color3DShader->bind();
	color3DShader->setModelViewMatrix(camera.getViewMatrix());
	color3DShader->unbind();




	gBuffer.startFrame();

	gBuffer.startGeometryPass();

	testMesh->render();
	testCharacter->render();
	if (cloth)
		cloth->render();

	//ikTest->render();

	terrain->render();

	gBuffer.endGeometryPass();

	/*
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);*/

	lights.render();

	gBuffer.endFrame();


	if (characterEditor3DProperties.getShowGBufferProperty()->getValue())
		gBuffer.renderBuffers();

	dynamicsWorld->debugDrawWorld();
	debugDraw.render();

	Renderer* renderer = spriteManager->getRenderer();
	renderer->enableBlending();
	renderer->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	windowManager.update();
	windowManager.draw();
	renderer->disableBlending();
}

/*void Test::frameRender() {
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	color3DShader->bind();
	color3DShader->setModelViewMatrix(camera.getViewMatrix());
	color3DShader->unbind();

	testMesh->render();

	testCharacter->render();

	if (bShowGBuffer) {
		gBuffer.bindForWriting();

		//glClearColor(1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		testCharacter->render();

		gBuffer.unbind();

		gBuffer.renderBuffers();
	}

	glDisable(GL_DEPTH_TEST);
	dynamicsWorld->debugDrawWorld();
	debugDraw.render();
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);
	windowManager.draw();
	glEnable(GL_DEPTH_TEST);
}*/
