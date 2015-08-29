#include "CharacterEditor.h"

CharacterEditor::CharacterEditor(SpriteShader* spriteShader, RadialBlurShader* radialBlurShader, FontShader* fontShader, ColorShader* colorShader,
					 ContextManager* contextManager, SpriteManager* spriteManager, FontManager* fontManager, int screenWidth, int screenHeight) :
	gameManager(spriteManager),
	boneManager(spriteManager->getRenderer(), &gameManager),
	boneSelection(&gameManager, &boneManager),
	characterEditorProperties(spriteManager, &actionManager, &boneManager, &boneSelection),
	//boneProperties(&actionManager, &boneManager, &boneSelection),
	windowManager(spriteManager),
	fboManager(spriteShader, radialBlurShader, screenWidth, screenHeight)
{
	boneSelection.setBoneProperties(characterEditorProperties.getBoneProperties());

	this->spriteShader = spriteShader;
	this->radialBlurShader = radialBlurShader;
	this->fontShader = fontShader;
	this->colorShader = colorShader;
	this->contextManager = contextManager;
	this->spriteManager = spriteManager;
	this->fontManager = fontManager;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -128.0f, 128.0f);
	glm::mat4 modelViewMatrix = glm::mat4();
	//glm::mat4 modelViewMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));

	viewPosX = viewPosY = 0.0f;


	//spriteShader.loadFromFiles("Calamity/Shader/sprite.vert", "Calamity/Shader/sprite.frag");

	spriteShader->bind();
	spriteShader->setProjectionMatrix(projectionMatrix);
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();

	radialBlurShader->bind();
	radialBlurShader->setProjectionMatrix(projectionMatrix);
	radialBlurShader->setModelViewMatrix(modelViewMatrix);
	radialBlurShader->unbind();

	fontShader->bind();
	fontShader->setProjectionMatrix(projectionMatrix);
	fontShader->setModelViewMatrix(modelViewMatrix);
	fontShader->unbind();

	colorShader->bind();
	colorShader->setProjectionMatrix(projectionMatrix);
	colorShader->setModelViewMatrix(modelViewMatrix);
	colorShader->unbind();

	Renderer* renderer = spriteManager->getRenderer();

	bgLayer.reset(new RenderLayer(renderer));
	moonLayer.reset(new RenderLayer(renderer));


	//spriteManager->addBackgroundSprite(screenWidth, screenHeight, "Data/moonTest.png", blurLayer);
	//spriteManager->addBackgroundSprite(screenWidth, screenHeight, "Data/moon9.png", spriteBackground);



	bg = spriteManager->addSprite(0, screenHeight, "Data/night_gradient.png", bgLayer.get());
	//bg = spriteManager->addSprite(0, screenHeight, "Data/sunset_gradient.png", bgLayer);
	//bg = spriteManager->addSprite(0, screenHeight, "Data/sunset_gradient3.png", bgLayer);
	spriteManager->setSpritePositionAndSize(bg, 0, screenHeight, screenWidth, screenHeight);



	float moonX = 600;
	float moonY = 700;
	moon = spriteManager->addSprite(moonX, moonY, "Data/Objects/moon.png", moonLayer.get());
	/*float moonX = 600;
	float moonY = 500;
	moon = spriteManager->addSprite(moonX, moonY, "Data/Objects/sun.png", moonLayer);*/
	//radialBlurShader->setBlurOrigin(moonX + 97, moonY - 97);


	// TODO: find out why there is a 1px space between textures in the atlas
	//spriteManager->addSprite(263, 517, "Data/ellie.png", moonLayer.get());
	//spriteManager->addSprite(263, 517, "Data/Sylvanas.png", moonLayer.get());


	//spriteSelection = new SpriteSelection(gameManager);
	backLayer = gameManager.addGameLayer(0);
	middleLayer = gameManager.addGameLayer(0.5f);
	frontLayer = gameManager.addGameLayer(1.0f);

	boneLayer = gameManager.addGameLayer(1.0f);


	//gameManager->addGameObject(cloud, layer1);



	/*boneManager = new BoneManager(spriteManager);
	boneManager->addBone(600.0f, 500.0f, 900.0f, 500.0f);
	boneManager->addBone(600.0f, 400.0f, 900.0f, 300.0f);*/


	//fboManager = new FboManager(spriteShader, radialBlurShader, screenWidth, screenHeight);
	backgroundLightFbo = fboManager.addFbo();

	actionHandler = nullptr;



	//initGui();

	fontId = fontManager->createFont("Fonts/DejaVuSans.ttf", 10);

	characterEditorProperties.setFontId(fontId);



	//spriteManager->uploadTextures();
	//renderer->writeTextureAtlasesToPng();
}

void CharacterEditor::initGui() {
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
	//Checkbox* checkTest = new Checkbox(spriteManager, 100, L"File  Edit  Options", fontId, nullptr, nullptr, true);
	//menuLayout->addChild(checkTest);
	ButtonGroup* editorModeButtons1 = new ButtonGroup(spriteManager, fontId, 200);
	menuLayout->addChild(editorModeButtons1);
	editorModeButtons1->addRow();
	editorModeButtons1->addButton(0, L"Save", nullptr);
	editorModeButtons1->addButton(0, L"Load", nullptr);
	editorModeButtons1->addButton(0, L"Options", nullptr);
	Checkbox* checkTest = new Checkbox(spriteManager, 100, L"Test", fontId, nullptr, nullptr, true);
	menuLayout->addChild(checkTest);
	SegmentControl* editorModeSC = new SegmentControl(spriteManager, fontId, 150, &characterEditorProperties, characterEditorProperties.getCharacterEditorModeProperty());
	menuLayout->addChild(editorModeSC);
	editorModeSC->addRow();
	editorModeSC->addSegment(0, L"Bones", BONES_MODE);
	editorModeSC->addSegment(0, L"Skin", SKIN_MODE);
	editorModeSC->addSegment(0, L"Pose", POSE_MODE);


	unsigned int leftWindowId = windowManager.createWindow(0, 57, 180, screenHeight - 57, CLM_BOTTOM_LEVEL | CLM_RESIZE_RIGHT | CLM_NO_MOVE);
	leftWindow = windowManager.getWindow(leftWindowId);
	leftWindow->setPadding(0);

	characterEditorProperties.setWindow(leftWindow);
	characterEditorProperties.loadBonesPanel();


	unsigned int rightWindowId = windowManager.createWindow(screenWidth - 180, 57, 180, screenHeight - 57, CLM_BOTTOM_LEVEL | CLM_RESIZE_LEFT | CLM_NO_MOVE);
	Window* rightWindow = windowManager.getWindow(rightWindowId);
	rightWindow->setPadding(0);

	VerticalBoxLayout* rightBoxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	rightWindow->setChild(rightBoxLayout);

	VerticalLayout* layersLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int  layerListBox = rightBoxLayout->addBox();
	rightBoxLayout->setBoxWidget(layerListBox, layersLayout);

	ListView* layerList = new ListView(spriteManager, 100, 100, true, true);
	layersLayout->addChild(layerList);

	ButtonGroup* layerButtons = new ButtonGroup(spriteManager, fontId, 150, true);
	layersLayout->addChild(layerButtons);
	layerButtons->addRow();
	layerButtons->addButton(0, L"Move up");
	layerButtons->addButton(0, L"Move down");
	layerButtons->addRow();
	layerButtons->addButton(1, L"Add layer");
	layerButtons->addButton(1, L"Remove layer");

	/*LineEdit* layerNameEdit = new LineEdit(spriteManager, 100, L"Layer", fontId, true);
	layersLayout->addChild(layerNameEdit);*/


	spriteManager->uploadTextures();
}

string CharacterEditor::getContextName() {
	string result = "2D Character Editor";
	return result;
}

bool CharacterEditor::handleEvent(SDL_Event* event) {
	if (actionHandler != nullptr) {
		if (actionHandler->handleEvent(event)) {
			delete actionHandler;
			actionHandler = nullptr;
		}
	}
	else {
		switch (event->type) {
			case SDL_MOUSEBUTTONDOWN:
				if (event->button.button == SDL_BUTTON_LEFT) {
					if (windowManager.contains(event->button.x, screenHeight - event->button.y)) {
						actionHandler = windowManager.createActionHandler(event->button.x, screenHeight - event->button.y, event);
					}
					else {
						float scrollSpeed = gameManager.getLayerScrollSpeed(boneLayer);
						float x = viewPosX * scrollSpeed + event->button.x;
						float y = viewPosY * scrollSpeed + screenHeight - event->button.y;

						list<Vec2> startingPositions;
						startingPositions.push_back(Vec2(x, y));
						actionHandler = new CreateBonesHandler(&boneManager, &boneSelection, characterEditorProperties.getLoadBoneListFunctor(), &actionManager, spriteManager, startingPositions, boneLayer);
					}
				}
				/*else if (event->button.button == SDL_BUTTON_MIDDLE) {
					actionHandler = new PanViewHandler(&viewPosX, &viewPosY);
				}*/
				else if (event->button.button == SDL_BUTTON_RIGHT) {
					if (windowManager.contains(event->button.x, screenHeight - event->button.y)) {
						actionHandler = windowManager.createActionHandler(event->button.x, screenHeight - event->button.y, event);
					}
					else {
						float scrollSpeed = gameManager.getLayerScrollSpeed(boneLayer);
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
						}
					}
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
						IntProperty* characterEditorModeProperty = characterEditorProperties.getCharacterEditorModeProperty();
						wstring newCharacterEditorMode = characterEditorProperties.getPreviousCharacterEditorMode();
						wstring oldCharacterEditorMode = characterEditorModeProperty->getValueAsString();
						characterEditorProperties.pushPropertyChange(characterEditorModeProperty, oldCharacterEditorMode, newCharacterEditorMode);
						break;
					}
					case SDLK_a:
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
					}
					case SDLK_e:
					{
						if (!boneSelection.getSelectedBones().empty()) {
							list<Vec2> startingPositions;
							for (auto& it : boneSelection.getSelectedBones()) {
								BoneObject* b = (BoneObject*)gameManager.getObject(it.first);
								if (it.second == 1)
									startingPositions.push_back(b->getRootPosition());
								else if (it.second == 2)
									startingPositions.push_back(b->getTipPosition());
								else if (it.second == 3) {
									startingPositions.push_back(b->getRootPosition());
									startingPositions.push_back(b->getTipPosition());
								}
							}
							actionHandler = new CreateBonesHandler(&boneManager, &boneSelection, characterEditorProperties.getLoadBoneListFunctor(), &actionManager, spriteManager, startingPositions, boneLayer);
						}
						break;
					}
					case SDLK_g:
					{
						if (!boneSelection.getSelectedBones().empty())
							actionHandler = new MoveBonesHandler(&boneSelection, &actionManager);
						break;
					}
					case SDLK_p:
					{
						//TODO: fix for removed bones
						unordered_map<unsigned int, int> selectedBones = boneSelection.getSelectedBones();
						if (selectedBones.size() > 1) {
							unsigned int mainSelectedBoneId = boneSelection.getMainSelectedBone();
							BoneObject* mainSelectedBone = (BoneObject*)gameManager.getObject(mainSelectedBoneId);

							SetBonesParent* action = new SetBonesParent(&boneSelection, mainSelectedBone);
							actionManager.pushAction(action);
						}
						break;
					}
					case SDLK_r:
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
							RemoveBones* action = new RemoveBones(&boneManager, &boneSelection, characterEditorProperties.getLoadBoneListFunctor());
							actionManager.pushAction(action);
						}
					}
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
		}
	}

	return false;
}

void CharacterEditor::update(float deltaTime) {

}

void CharacterEditor::frameRender() {
	bgLayer->clearLayers();
	moonLayer->clearLayers();

	gameManager.clearLayer(backLayer);
	gameManager.clearLayer(middleLayer);
	gameManager.clearLayer(frontLayer);
	gameManager.clearLayer(boneLayer);


	glClearColor(0, 0, 0, 0);

	glm::mat4 modelViewMatrix;

	Renderer* renderer = spriteManager->getRenderer();
	renderer->enableBlending();
	renderer->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//spriteManager->flagAllForRender();
	spriteManager->flagSpriteForRender(bg);
	/*spriteManager->flagSpriteForRender(stars1);
	spriteManager->flagSpriteForRender(stars2);
	spriteManager->flagSpriteForRender(stars3);*/
	spriteManager->flagSpriteForRender(moon);
	//spriteManager->flagLineForRender(line1);
	bgLayer->uploadLayerBuffers();
	moonLayer->uploadLayerBuffers();
	gameManager.prepareLayer(backLayer, viewPosX, viewPosY, 0);
	gameManager.prepareLayer(middleLayer, viewPosX, viewPosY, 0);
	gameManager.prepareLayer(frontLayer, viewPosX, viewPosY, 0);
	gameManager.prepareLayer(boneLayer, viewPosX, viewPosY, 0);

	fboManager.bindFbo(backgroundLightFbo);

	//glClearColor(0.1f, 0.1f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelViewMatrix = glm::mat4();
	spriteShader->bind();
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();
	moonLayer->draw();

	spriteShader->bind();
	spriteShader->setRenderBlack(true);
	spriteShader->unbind();


	modelViewMatrix = glm::translate(glm::mat4(), glm::vec3(-viewPosX, -viewPosY, 0.0f));
	spriteShader->bind();
	//spriteShader->setRenderBlack(true);
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();
	fontShader->bind();
	fontShader->setModelViewMatrix(modelViewMatrix);
	fontShader->unbind();
	colorShader->bind();
	colorShader->setModelViewMatrix(modelViewMatrix);
	colorShader->unbind();


	gameManager.drawLayer(backLayer, 0);
	gameManager.drawLayer(middleLayer, 0);
	gameManager.drawLayer(frontLayer, 0);


	spriteShader->bind();
	spriteShader->setRenderBlack(false);
	spriteShader->unbind();


	fboManager.unbindFbo();

	//glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelViewMatrix = glm::mat4();
	spriteShader->bind();
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();
	/*bgLayer->draw();
	moonLayer->draw();*/



	modelViewMatrix = glm::translate(glm::mat4(), glm::vec3(-viewPosX, -viewPosY, 0.0f));
	spriteShader->bind();
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();


	gameManager.drawLayer(backLayer, 0);
	gameManager.drawLayer(middleLayer, 0);
	gameManager.drawLayer(frontLayer, 0);

	gameManager.drawOverlay(backLayer, 0);
	gameManager.drawOverlay(middleLayer, 0);
	gameManager.drawOverlay(frontLayer, 0);


	gameManager.drawOverlay(boneLayer, 0);


	modelViewMatrix = glm::mat4();
	spriteShader->bind();
	//spriteShader.setRenderBlack(false);
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();
	fontShader->bind();
	fontShader->setModelViewMatrix(modelViewMatrix);
	fontShader->unbind();
	/*glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_DST_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE);*/
	renderer->setBlendFunc(GL_ONE, GL_ONE);
	fboManager.drawFbo(backgroundLightFbo);
	//glDisable(GL_BLEND);
	renderer->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//boneManager->drawBoneLayer();

	windowManager.update();
	windowManager.draw();

	// moved to update
	/*bgLayer->clearLayers();
	moonLayer->clearLayers();

	gameManager.clearLayer(backLayer);
	gameManager.clearLayer(middleLayer);
	gameManager.clearLayer(frontLayer);
	gameManager.clearLayer(boneLayer);*/


	renderer->disableBlending();
}
