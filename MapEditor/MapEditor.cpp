#include "MapEditor.h"

MapEditor::MapEditor(SpriteShader* spriteShader, TextureShader* textureShader, RadialBlurShader* radialBlurShader,
					 FontShader* fontShader, ColorShader* colorShader, ContextManager* contextManager,
					 SpriteManager* spriteManager, FontManager* fontManager, int screenWidth, int screenHeight) :
	gameManager(spriteManager),
	layerManager(spriteManager->getRenderer(), &gameManager),
	layerProperties(&actionManager, &layerManager, &gameManager),
	mapEditorSelection(&gameManager, spriteManager, &sharedTriangleData),
	mapEditorProperties(spriteManager, &mapEditorSelection),
	mapObjectProperties(&actionManager, &mapEditorSelection),
	windowManager(spriteManager),
	fboManager(spriteShader, radialBlurShader, screenWidth, screenHeight)
{
	mapEditorSelection.setMapObjectProperties(&mapObjectProperties);

	this->spriteShader = spriteShader;
	this->textureShader = textureShader;
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

	textureShader->bind();
	textureShader->setProjectionMatrix(projectionMatrix);
	textureShader->setModelViewMatrix(modelViewMatrix);
	textureShader->unbind();

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



	//bg = spriteManager->addSprite(0, screenHeight, "Data/night_gradient.png", bgLayer.get());
	//bg = spriteManager->addSprite(0, screenHeight, "Data/sunset_gradient.png", bgLayer.get());
	bg = spriteManager->addSprite(0, screenHeight, "Data/sunset_gradient3.png", bgLayer.get());
	spriteManager->setSpritePositionAndSize(bg, 0, screenHeight, screenWidth, screenHeight);

	/*float moonX = 600;
	float moonY = 700;
	moon = spriteManager->addSprite(moonX, moonY, "Data/Objects/moon.png", moonLayer.get());*/
	float moonX = 600;
	float moonY = 500;
	moon = spriteManager->addSprite(moonX, moonY, "Data/Objects/sun.png", moonLayer.get());
	radialBlurShader->setBlurOrigin(moonX + 97, moonY - 97);


	// TODO: find out why there is a 1px space between textures in the atlas
	/*spriteManager->addSprite(263, 517, "Data/ellie.png", moonLayer);
	spriteManager->addSprite(263, 517, "Data/Sylvanas.png", moonLayer);*/


	layerManager.addLayer(L"Back", 0.0f);
	layerManager.addLayer(L"Middle", 0.5f);
	layerManager.addLayer(L"Front", 1.0f);
	lightLayer = layerManager.addLayer(L"Lights", 1.0f);


	backgroundLightFbo = fboManager.addFbo();
	backgroundLightFbo2 = fboManager.addFbo();
	lightFbo = fboManager.addFboFloatingPoint();

	actionHandler = nullptr;

	fontId = fontManager->createFont("Fonts/DejaVuSans.ttf", 10);

	mapEditorProperties.setFontId(fontId);


	//eyeOffset = 8;

	//printf("%d", GL_MAX_TEXTURE_SIZE);

	/*textLayer = new RenderLayer(renderer);
	font1 = fontManager->createFont("Fonts/DejaVuSans.ttf", 13);
	font2 = fontManager->createFont("Fonts/Minecraft Evenings.otf", 50);
	//font2 = fontManager->createFont("Fonts/RIKY2vamp.ttf", 50);
	font3 = fontManager->createFont("Fonts/trashco.ttf", 60);
	//fontManager->writeToPng(font2);

	text1 = spriteManager->addText(L"Maximilian Jensen", font1, 220, 270, 0, 0, 0, 255, textLayer);
	text2 = spriteManager->addText(L"Maximilian Jensen", font2, 220, 220, 0, 0, 0, 255, textLayer);
	text3 = spriteManager->addText(L"Maximilian Jensen", font3, 220, 170, 0, 0, 0, 255, textLayer);*/


	//line1 = spriteManager->addLine(200, 200, 400, 500, 255, 255, 255, 255, 255, 255, 255, 255, textLayer);



	//spriteManager->uploadTextures();
	//renderer->writeTextureAtlasesToPng();


	leftColorMask[0] = true;
	leftColorMask[1] = false;
	leftColorMask[2] = false;

	rightColorMask[0] = false;
	rightColorMask[1] = true;
	rightColorMask[2] = true;

	combinedColorMask[0] = true;
	combinedColorMask[1] = true;
	combinedColorMask[2] = true;
}

void MapEditor::initGui() {
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
	InputGroup* eyeOffsetInput = new InputGroup(spriteManager, fontId, 150);
	menuLayout->addChild(eyeOffsetInput);
	eyeOffsetInput->addRow();
	eyeOffsetInput->addInput(0, L"EyeOffset", &mapEditorProperties, mapEditorProperties.getEyeOffsetProperty());
	Checkbox* checkTest = new Checkbox(spriteManager, 100, L"Anaglyph 3D", fontId, &mapEditorProperties, mapEditorProperties.getAnaglyph3DProperty(), true);
	menuLayout->addChild(checkTest);
	SegmentControl* editorModeSC = new SegmentControl(spriteManager, fontId, 150, &mapEditorProperties, mapEditorProperties.getEditorModeProperty());
	menuLayout->addChild(editorModeSC);
	editorModeSC->addRow();
	editorModeSC->addSegment(0, L"Polygons", POLYGON_MODE);
	editorModeSC->addSegment(0, L"Objects", OBJECT_MODE);
	/*editorModeButtons->addRow();
	editorModeButtons->addButton(1, L"Polygons", nullptr);
	editorModeButtons->addButton(1, L"Objects", nullptr);*/


	//unsigned int leftWindowId = windowManager->createWindow(20, 20, 180, screenHeight - 40);
	unsigned int leftWindowId = windowManager.createWindow(0, 57, 180, screenHeight - 57, CLM_BOTTOM_LEVEL | CLM_RESIZE_RIGHT | CLM_NO_MOVE);
	Window* leftWindow = windowManager.getWindow(leftWindowId);
	//window = new Window(spriteManager, 20, 20, 200, 400);
	leftWindow->setPadding(0);

	mapEditorProperties.setWindow(leftWindow);
	mapEditorProperties.loadPolygonPanel();

	/*verticalBoxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	window->setChild(verticalBoxLayout);
	testWidget1 = new TestWidget(spriteManager, 100, 100, true, true);
	testWidget2 = new TestWidget(spriteManager, 100, 100, true, true);
	verticalBoxLayout->addChild(testWidget1);
	verticalBoxLayout->addChild(testWidget2);*/

	//window->setChild(horizontalLayout);
	/*verticalLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	window->setChild(verticalLayout);
	horizontalLayout = new HorizontalLayout(spriteManager, 0, 0, true, true);
	testWidget1 = new TestWidget(spriteManager, 100, 100, true);
	testWidget2 = new TestWidget(spriteManager, 50, 50, true, true);
	testWidget3 = new TestWidget(spriteManager, 50, 50, true, false);
	testWidget4 = new TestWidget(spriteManager, 50, 50, true);
	verticalLayout->addChild(testWidget1);
	verticalLayout->addChild(horizontalLayout);
	horizontalLayout->addChild(testWidget2);
	horizontalLayout->addChild(testWidget3);
	verticalLayout->addChild(testWidget4);*/

	/*VerticalBoxLayout* leftBoxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	leftWindow->setChild(leftBoxLayout);

	ListView* objectList = new ListView(spriteManager, 100, 100, true, true);
	unsigned int objectListBox = leftBoxLayout->addBox();
	leftBoxLayout->setBoxWidget(objectListBox, objectList);

	{
		DIR *dp;
		struct dirent *dirp;
		//dp  = opendir((string("/home/max/projects/mt/build/").append(folderName)).c_str());
		dp  = opendir(string("Data/Objects").c_str());
		if (dp == NULL)
			printf("Unable to open Data/Objects");

		list<string> fileNames;
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_name[0] != '.') {
				string fileName = dirp->d_name;
				fileNames.push_back(fileName);
			}
		}
		closedir(dp);
		fileNames.sort();
		for (auto& it : fileNames) {
			ObjectItem* item = new ObjectItem(spriteManager, &cObjectFileName, it.c_str(), fontId);
			objectList->addItem(item);
		}
	}

	unsigned int objectPropertiesBox = leftBoxLayout->addBox();
	spriteSelection.setObjectPropertiesBox(leftBoxLayout, objectPropertiesBox, fontId);*/

	/*HorizontalLayout* horizontalLayout = new HorizontalLayout(spriteManager, 0, 0, true, true);
	unsigned int testBox1 = leftBoxLayout->addBox();
	leftBoxLayout->setBoxWidget(testBox1, horizontalLayout);
	//leftBoxLayout->addChild(horizontalLayout);
	//testWidget1 = new TestWidget(spriteManager, 100, 100, true, true);
	TestWidget* testWidget2 = new TestWidget(spriteManager, 50, 50, true, true);
	TestWidget* testWidget3 = new TestWidget(spriteManager, 50, 50, true, false);
	horizontalLayout->addChild(testWidget2);
	horizontalLayout->addChild(testWidget3);

	TestWidget* testWidget4 = new TestWidget(spriteManager, 50, 50, true);
	unsigned int testBox2 = leftBoxLayout->addBox();
	leftBoxLayout->setBoxWidget(testBox2, testWidget4);
	//leftBoxLayout->addChild(testWidget4);*/


	//unsigned int rightWindowId = windowManager->createWindow(screenWidth - 200, 20, 180, screenHeight - 40);
	unsigned int rightWindowId = windowManager.createWindow(screenWidth - 180, 57, 180, screenHeight - 57, CLM_BOTTOM_LEVEL | CLM_RESIZE_LEFT | CLM_NO_MOVE);
	Window* rightWindow = windowManager.getWindow(rightWindowId);
	rightWindow->setPadding(0);

	VerticalBoxLayout* rightBoxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	rightWindow->setChild(rightBoxLayout);

	VerticalLayout* layersLayout = new VerticalLayout(spriteManager, 0, 0, true, true);
	unsigned int layerListBox = rightBoxLayout->addBox();
	rightBoxLayout->setBoxWidget(layerListBox, layersLayout);
	unsigned int layerPropertiesBox = rightBoxLayout->addBox();
	//rightBoxLayout->addChild(layersLayout);

	/*testFunctor.setBoxLayout(rightBoxLayout);
	testFunctor.setBox(layerListBox);*/
	//selectLayerFunctor = new SelectLayerFunctor(gameManager, layerManager, spriteManager, rightBoxLayout, layerPropertiesBox, fontId);
	selectLayerFunctor.reset(new SelectLayerFunctor(&gameManager, &layerManager, &layerProperties, spriteManager, rightBoxLayout, layerPropertiesBox, fontId));

	ListView* layerList = new ListView(spriteManager, 100, 100, true, true);
	layersLayout->addChild(layerList);

	loadLayerListFunctor.reset(new LoadLayerListFunctor(layerList, &layerManager, spriteManager, fontId, selectLayerFunctor.get()));
	layerList->setLoadFunctor(loadLayerListFunctor.get());
	layerList->reload();
	layerProperties.setLayerList(layerList);

	/*list<unsigned int> layerIds = layerManager.getLayerIds();
	for (auto& it : layerIds) {
		wstring label = layerManager.getLayerName(it);
		LayerItem* item = new LayerItem(spriteManager, &layerManager, label, it, fontId);
		item->setFunctor(selectLayerFunctor.get());
		layerList->addItem(item);
	}*/
	createLayerFunctor.reset(new CreateLayerFunctor(&actionManager, &layerManager, layerList, fontId, selectLayerFunctor.get()));
	removeLayerFunctor.reset(new RemoveLayerFunctor(&actionManager, &layerManager, &mapEditorSelection, layerList, fontId, selectLayerFunctor.get()));
	moveLayerUpFunctor.reset(new MoveLayerFunctor(&actionManager, &layerManager, layerList, selectLayerFunctor.get(), true));
	moveLayerDownFunctor.reset(new MoveLayerFunctor(&actionManager, &layerManager, layerList, selectLayerFunctor.get(), false));

	ButtonGroup* layerButtons = new ButtonGroup(spriteManager, fontId, 150, true);
	layersLayout->addChild(layerButtons);
	layerButtons->addRow();
	layerButtons->addButton(0, L"Move up", moveLayerUpFunctor.get());
	layerButtons->addButton(0, L"Move down", moveLayerDownFunctor.get());
	layerButtons->addRow();
	layerButtons->addButton(1, L"Add layer", createLayerFunctor.get());
	layerButtons->addButton(1, L"Remove layer", removeLayerFunctor.get());

	/*LineEdit* layerNameEdit = new LineEdit(spriteManager, 100, L"Layer", fontId, &layerProperties, layerProperties.getLayerNameProperty(), true);
	layersLayout->addChild(layerNameEdit);*/
	//rightBoxLayout->addChild(layerNameEdit);

	//TestWidget* testWidget5 = new TestWidget(spriteManager, 50, 50, true);
	/*InputGroup* testInput = new InputGroup(spriteManager, fontId, 150, true);
	testInput->addRow();
	testInput->addInput(0, L"x", CLM_FLOAT);
	testInput->addInput(0, L"y", CLM_INT);
	testInput->addRow();
	testInput->addInput(1, L"ScrollSpeed", CLM_FLOAT);

	rightBoxLayout->setBoxWidget(layerPropertiesBox, testInput);*/


	//rightBoxLayout->clearBox(layerPropertiesBox);
	/*InputGroup* scrollSpeedInput = new InputGroup(spriteManager, fontId, 150, true);
	rightBoxLayout->setBoxWidget(layerPropertiesBox, scrollSpeedInput);
	scrollSpeedInput->addRow();
	scrollSpeedInput->addInput(0, L"ScrollSpeed", CLM_FLOAT);*/

	//rightBoxLayout->addChild(testWidget5);

	//rightWindow->deleteChild();

	spriteManager->uploadTextures();
}

string MapEditor::getContextName() {
	string result = "2D Map Editor";
	return result;
}

bool MapEditor::handleEvent(SDL_Event* event) {
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
						unsigned int selectedLayerId = layerManager.getSelectedLayerId();
						if (selectedLayerId != (unsigned int)-1) {
							float scrollSpeed = gameManager.getLayerScrollSpeed(selectedLayerId);
							float posX = viewPosX * scrollSpeed + event->button.x;
							float posY = viewPosY * scrollSpeed + screenHeight - event->button.y;
							if (mapEditorProperties.getEditorModeProperty()->getValue() == POLYGON_MODE) {
								string cTextureFileName = mapEditorProperties.getCTextureFileName();
								if (cTextureFileName != "") {
									string fileName = "Data/Textures/";
									fileName.append(cTextureFileName);
									actionHandler = new CreateTriangleHandler(&mapEditorSelection, &actionManager, selectedLayerId, fileName.c_str(), posX, posY);
								}
							}
							else if (mapEditorProperties.getEditorModeProperty()->getValue() == OBJECT_MODE) {
								string cObjectFileName = mapEditorProperties.getCObjectFileName();
								if (cObjectFileName != "") {
									string fileName = "Data/Objects/";
									fileName.append(cObjectFileName);
									CreateSpriteObject* action = new CreateSpriteObject(spriteManager, &mapEditorSelection, posX, posY, fileName.c_str(), selectedLayerId);
									actionManager.pushAction(action);
									spriteManager->uploadTextures();
								}
							}
						}
					}

				}
				else if (event->button.button == SDL_BUTTON_MIDDLE) {
					actionHandler = new PanViewHandler(&viewPosX, &viewPosY);
				}
				else if (event->button.button == SDL_BUTTON_RIGHT) {
					if (windowManager.contains(event->button.x, screenHeight - event->button.y)) {
						actionHandler = windowManager.createActionHandler(event->button.x, screenHeight - event->button.y, event);
					}
					else {
					unsigned int selectedLayerId = layerManager.getSelectedLayerId();
						if (selectedLayerId != (unsigned int)-1) {
							float scrollSpeed = gameManager.getLayerScrollSpeed(selectedLayerId);
							float x = viewPosX * scrollSpeed + event->button.x;
							float y = viewPosY * scrollSpeed + screenHeight - event->button.y;
							list<unsigned int> possibleObjects = gameManager.getObjectsAtPosition(selectedLayerId, x, y);
							if (mapEditorProperties.getEditorModeProperty()->getValue() == POLYGON_MODE) {
								unordered_map<unsigned int, unsigned int> verticesToSelect;
								//vector<unsigned int> trianglesToUpdate;
								float minDistanceSq = 400.0f;
								for (auto& it : possibleObjects) {
									GameObject* possibleObject = gameManager.getObject(it);
									if (possibleObject->getObjectType() == TRIANGLE_OBJECT) {
										pair<unsigned int, float> v = ((TriangleObject*)possibleObject)->getClosestVertex(x, y);
										if (v.first != (unsigned int)-1) {
											if (v.second == minDistanceSq) {
												verticesToSelect[v.first] = selectedLayerId;
												//trianglesToUpdate.push_back(it);
											}
											else if (v.second < minDistanceSq) {
												verticesToSelect.clear();
												//trianglesToUpdate.clear();

												verticesToSelect[v.first] = selectedLayerId;
												//trianglesToUpdate.push_back(it);
												minDistanceSq = v.second;
											}
										}
									}
								}
								if (!verticesToSelect.empty()) {
									const Uint8* keyState = SDL_GetKeyboardState(NULL);
									SelectObjects* action;
									if (keyState[SDL_SCANCODE_LSHIFT])
										action = new SelectObjects(&mapEditorSelection);
									else
										action = new SelectObjects(&mapEditorSelection, true);
									action->setVerticesToSelect(verticesToSelect);
									actionManager.pushAction(action);
								}
								/*for (auto& it : verticesToSelect) {
									mapEditorSelection.selectVertex(it, trianglesToUpdate, selectedLayerId);
								}*/
							}
							else if (mapEditorProperties.getEditorModeProperty()->getValue() == OBJECT_MODE) {
								list<unsigned int> objects;
								for (auto& it : possibleObjects) {
									GameObject* possibleObject = gameManager.getObject(it);
									if (possibleObject->containsPoint(x, y) && possibleObject->getObjectType() == SPRITE_OBJECT)
										objects.push_back(it);
								}
								if (!objects.empty()) {
									const Uint8* keyState = SDL_GetKeyboardState(NULL);
									SelectObjects* action;
									if (keyState[SDL_SCANCODE_LSHIFT])
										action = new SelectObjects(&mapEditorSelection);
									else
										action = new SelectObjects(&mapEditorSelection, true);
									action->setSpritesToSelect(objects);
									actionManager.pushAction(action);
								}
							}
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
						IntProperty* editorModeProperty = mapEditorProperties.getEditorModeProperty();
						wstring newEditorMode = mapEditorProperties.getPreviousEditorMode();
						wstring oldEditorMode = editorModeProperty->getValueAsString();
						mapEditorProperties.pushPropertyChange(editorModeProperty, oldEditorMode, newEditorMode);
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
							list<unsigned int> selectedSprites = mapEditorSelection.getSelectedSprites();
							unordered_map<unsigned int, unsigned int> selectedVertices = mapEditorSelection.getSelectedVertices();
							if (!selectedSprites.empty() || !selectedVertices.empty()) {
								SelectObjects* action = new SelectObjects(&mapEditorSelection);
								action->setSpritesToSelect(selectedSprites);
								action->setVerticesToSelect(selectedVertices);
								actionManager.pushAction(action);
							}
						}
						break;
					}
					case SDLK_f:
					{
						string cTextureFileName = mapEditorProperties.getCTextureFileName();
						if (cTextureFileName != "") {
							string fileName = "Data/Textures/";
							fileName.append(cTextureFileName);
							unordered_map<unsigned int, unsigned int> selectedVertices = mapEditorSelection.getSelectedVertices();
							if (selectedVertices.size() == 3) {
								bool bFillable = true;
								unordered_map<unsigned int, unsigned int>::iterator it = selectedVertices.begin();

								// check for same layer
								unsigned int layerId = it->second;
								for (auto& it : selectedVertices) {
									if (it.second != layerId)
										bFillable = false;
								}

								// check if triangle already exists
								if (bFillable) {
									Vec2 pos = sharedTriangleData.getSharedVertexPosition(it->first);
									list<unsigned int> objects = gameManager.getObjectsAtPosition(layerId, pos.x, pos.y);
									for (auto& it : objects) {
										GameObject* cObject = gameManager.getObject(it);
										if (cObject->getObjectType() == TRIANGLE_OBJECT) {
											TriangleObject* cTriangle = (TriangleObject*)cObject;
											int verticesContained = 0;
											for (auto& it2 : selectedVertices) {
												if (cTriangle->containsVertex(it2.first))
													++verticesContained;
											}
											if (verticesContained == 3) {
												bFillable = false;
												break;
											}
										}
									}

									if (bFillable) {
										FillTriangle* action = new FillTriangle(&mapEditorSelection, fileName.c_str());
										actionManager.pushAction(action);
									}
								}
							}
						}
						break;
					}
					case SDLK_g:
					{
						list<unsigned int> selectedSprites = mapEditorSelection.getSelectedSprites();
						unordered_map<unsigned int, unsigned int> selectedVertices = mapEditorSelection.getSelectedVertices();
						if (!selectedSprites.empty() || !selectedVertices.empty())
							actionHandler = new MoveObjectsHandler(&mapEditorSelection, &actionManager);
						break;
					}
					case SDLK_r:
					{
						list<unsigned int> selectedSprites = mapEditorSelection.getSelectedSprites();
						unordered_map<unsigned int, unsigned int> selectedVertices = mapEditorSelection.getSelectedVertices();
						if (!selectedSprites.empty() || selectedVertices.size() > 1) {
							unsigned int selectedLayerId = layerManager.getSelectedLayerId();
							if (selectedLayerId != (unsigned int)-1) {
								float scrollSpeed = gameManager.getLayerScrollSpeed(selectedLayerId);
								int x, y;
								SDL_GetMouseState(&x, &y);
								float mouseX = viewPosX * scrollSpeed + x;
								float mouseY = viewPosY * scrollSpeed + screenHeight - y;
								actionHandler = new RotateObjectsHandler(&mapEditorSelection, &actionManager, mouseX, mouseY);
							}
						}
						break;
					}
					case SDLK_s:
					{
						list<unsigned int> selectedSprites = mapEditorSelection.getSelectedSprites();
						unordered_map<unsigned int, unsigned int> selectedVertices = mapEditorSelection.getSelectedVertices();
						if (!selectedSprites.empty() || selectedVertices.size() > 1) {
							unsigned int selectedLayerId = layerManager.getSelectedLayerId();
							if (selectedLayerId != (unsigned int)-1) {
								float scrollSpeed = gameManager.getLayerScrollSpeed(selectedLayerId);
								int x, y;
								SDL_GetMouseState(&x, &y);
								float mouseX = viewPosX * scrollSpeed + x;
								float mouseY = viewPosY * scrollSpeed + screenHeight - y;
								actionHandler = new ScaleObjectsHandler(&mapEditorSelection, &actionManager, mouseX, mouseY);
							}
						}
						break;
					}
					case SDLK_x:
					{
						list<unsigned int> selectedSprites = mapEditorSelection.getSelectedSprites();
						unordered_map<unsigned int, unsigned int> selectedVertices = mapEditorSelection.getSelectedVertices();
						if (!selectedSprites.empty() || !selectedVertices.empty()) {
							RemoveObjects* action = new RemoveObjects(&mapEditorSelection);
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

void MapEditor::update(float deltaTime) {
	if (actionHandler != nullptr)
		actionHandler->update();
}

void MapEditor::frameRender() {
	bgLayer->clearLayers();
	moonLayer->clearLayers();
	layerManager.clearLayers();

	if (actionHandler != nullptr)
		actionHandler->render();

	mapEditorSelection.flagForRender();

	if (mapEditorProperties.getAnaglyph3DProperty()->getValue())
		layerManager.prepareLayers(viewPosX, viewPosY, mapEditorProperties.getEyeOffsetProperty()->getValue());
		//layerManager.prepareLayers(viewPosX, viewPosY, eyeOffset);
	else
		layerManager.prepareLayers(viewPosX, viewPosY);

	//mapEditorSelection.flagForRender(); //TODO: investigate why this breaks stuff

	spriteManager->flagSpriteForRender(bg);
	spriteManager->flagSpriteForRender(moon);
	bgLayer->uploadLayerBuffers();
	moonLayer->uploadLayerBuffers();



	bool bAnaglyph3D = mapEditorProperties.getAnaglyph3DProperty()->getValue();
	float eyeOffset = mapEditorProperties.getEyeOffsetProperty()->getValue();
	glm::mat4 modelViewMatrix;

	if (bAnaglyph3D) {
		colorShader->bind();
		colorShader->setAnaglyph(true);
		colorShader->unbind();
		spriteShader->bind();
		spriteShader->setAnaglyph(true);
		spriteShader->unbind();
		textureShader->bind();
		textureShader->setAnaglyph(true);
		textureShader->unbind();
	}

	Renderer* renderer = spriteManager->getRenderer();
	renderer->enableBlending();
	renderer->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0, 0, 0, 0);
	fboManager.bindFbo(lightFbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	layerManager.drawLayer(lightLayer);
	fboManager.unbindFbo();

	if (bAnaglyph3D) {
		fboManager.bindFbo(backgroundLightFbo);
		glColorMask(true, true, true, true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glColorMask(combinedColorMask[0], combinedColorMask[1], combinedColorMask[2], true);

		modelViewMatrix = glm::mat4();
		spriteShader->bind();
		spriteShader->setModelViewMatrix(modelViewMatrix);
		spriteShader->unbind();
		moonLayer->draw();

		spriteShader->bind();
		spriteShader->setRenderBlack(true);
		spriteShader->unbind();

		textureShader->bind();
		textureShader->setRenderBlack(true);
		textureShader->unbind();

		layerManager.drawLayers(-eyeOffset);

		spriteShader->bind();
		spriteShader->setRenderBlack(false);
		spriteShader->unbind();


		fboManager.bindFbo(backgroundLightFbo2);
		glColorMask(true, true, true, true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glColorMask(combinedColorMask[0], combinedColorMask[1], combinedColorMask[2], true);

		modelViewMatrix = glm::mat4();
		spriteShader->bind();
		spriteShader->setModelViewMatrix(modelViewMatrix);
		spriteShader->unbind();
		moonLayer->draw();

		spriteShader->bind();
		spriteShader->setRenderBlack(true);
		spriteShader->unbind();

		layerManager.drawLayers(eyeOffset);

		spriteShader->bind();
		spriteShader->setRenderBlack(false);
		spriteShader->unbind();

		textureShader->bind();
		textureShader->setRenderBlack(false);
		textureShader->unbind();


		fboManager.unbindFbo();

		glColorMask(true, true, true, true);
	}
	else {
		fboManager.bindFbo(backgroundLightFbo);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelViewMatrix = glm::mat4();
		spriteShader->bind();
		spriteShader->setModelViewMatrix(modelViewMatrix);
		spriteShader->unbind();
		moonLayer->draw();

		spriteShader->bind();
		spriteShader->setRenderBlack(true);
		spriteShader->unbind();

		textureShader->bind();
		textureShader->setRenderBlack(true);
		textureShader->unbind();

		layerManager.drawLayers();


		spriteShader->bind();
		spriteShader->setRenderBlack(false);
		spriteShader->unbind();

		textureShader->bind();
		textureShader->setRenderBlack(false);
		textureShader->unbind();


		fboManager.unbindFbo();
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelViewMatrix = glm::mat4();
	spriteShader->bind();
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();
	if (bAnaglyph3D) {
		glColorMask(combinedColorMask[0], combinedColorMask[1], combinedColorMask[2], true);
		bgLayer->draw();
		moonLayer->draw();
		glColorMask(true, true, true, true);
	}
	else {
		bgLayer->draw();
		moonLayer->draw();
	}


	if (bAnaglyph3D) {
		glColorMask(leftColorMask[0], leftColorMask[1], leftColorMask[2], true);
		layerManager.drawLayers(-eyeOffset);
		layerManager.drawOverlays(-eyeOffset);

		glColorMask(rightColorMask[0], rightColorMask[1], rightColorMask[2], true);
		layerManager.drawLayers(eyeOffset);
		layerManager.drawOverlays(eyeOffset);

		glColorMask(true, true, true, true);
	}
	else {
		layerManager.drawLayers();
		layerManager.drawOverlays();
	}



	/*glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_DST_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE);*/
	renderer->setBlendFunc(GL_ONE, GL_ONE);
	if (bAnaglyph3D)
	{
		glColorMask(leftColorMask[0], leftColorMask[2], leftColorMask[3], true);
		fboManager.drawFbo(backgroundLightFbo);

		glColorMask(rightColorMask[0], rightColorMask[2], rightColorMask[3], true);
		fboManager.drawFbo(backgroundLightFbo2);

		glColorMask(true, true, true, true);
	}
	else
		fboManager.drawFbo(backgroundLightFbo);
	//glDisable(GL_BLEND);
	renderer->setBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//fboManager.drawFbo(lightFbo);

	if (bAnaglyph3D) {
		colorShader->bind();
		colorShader->setAnaglyph(false);
		colorShader->unbind();
		spriteShader->bind();
		spriteShader->setAnaglyph(false);
		spriteShader->unbind();
		textureShader->bind();
		textureShader->setAnaglyph(false);
		textureShader->unbind();
	}


	modelViewMatrix = glm::mat4();
	spriteShader->bind();
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();
	fontShader->bind();
	fontShader->setModelViewMatrix(modelViewMatrix);
	fontShader->unbind();
	windowManager.update();
	windowManager.draw();


	// moved to update
	/*bgLayer->clearLayers();
	moonLayer->clearLayers();

	layerManager.clearLayers();*/


	renderer->disableBlending();
}
