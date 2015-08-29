#include "MapEditorSelection.h"

MapEditorSelection::MapEditorSelection(GameManager* gameManager, SpriteManager* spriteManager, SharedTriangleData* sharedTriangleData) :
	gameManager(gameManager),
	spriteManager(spriteManager),
	sharedTriangleData(sharedTriangleData),
	selectedSprites(),
	mainSelectedSprite(-1),
	selectedVertices(),
	selectedTriangles(),
	selectionEdges(),
	mapObjectProperties(nullptr),
	boxLayout(nullptr),
	boxId(0),
	fontId(0)
{}

void MapEditorSelection::setMapObjectProperties(MapObjectProperties* mapObjectProperties) {
	this->mapObjectProperties = mapObjectProperties;
}

void MapEditorSelection::updateProperties() {
	if (mapObjectProperties)
		mapObjectProperties->updateProperties();
}

void MapEditorSelection::setBoxWidget(VerticalBoxLayout* boxLayout, unsigned int boxId, unsigned int fontId) {
	this->boxLayout = boxLayout;
	this->boxId = boxId;
	this->fontId = fontId;
	loadBox();
}

void MapEditorSelection::loadBox() {
	if (boxLayout) {
		boxLayout->clearBox(boxId);
		if (mainSelectedSprite != (unsigned int)-1) {
			InputGroup* objectTransformInput = new InputGroup(spriteManager, fontId, 150, true);
			boxLayout->setBoxWidget(boxId, objectTransformInput);
			objectTransformInput->addRow();
			objectTransformInput->addInput(0, L"X", mapObjectProperties, mapObjectProperties->getXProperty());
			objectTransformInput->addInput(0, L"Y", mapObjectProperties, mapObjectProperties->getYProperty());
			objectTransformInput->addRow();
			objectTransformInput->addInput(1, L"Scale", mapObjectProperties, mapObjectProperties->getScaleProperty());
			objectTransformInput->addRow();
			objectTransformInput->addInput(2, L"Rotation", mapObjectProperties, mapObjectProperties->getRotationProperty());
			objectTransformInput->initialize();
		}
	}
}

void MapEditorSelection::selectSprite(unsigned int spriteId) {
	//((SpriteObject*)gameManager->getObject(spriteId))->toggleSelection();
	if (selectedSprites.find(spriteId) != selectedSprites.end()) {
		if (mainSelectedSprite == spriteId)
			setMainSelectedSprite(-1);
			//mainSelectedSprite = -1;
		((SpriteObject*)gameManager->getObject(spriteId))->setSelectionState(0);
		selectedSprites.erase(spriteId);
	}
	else {
		selectedSprites.insert(spriteId);
		((SpriteObject*)gameManager->getObject(spriteId))->setSelectionState(1);
		/*((SpriteObject*)gameManager->getObject(spriteId))->setSelectionState(2);
		if (mainSelectedSprite != (unsigned int)-1)
			((SpriteObject*)gameManager->getObject(mainSelectedSprite))->setSelectionState(1);
		mainSelectedSprite = spriteId;*/
	}
}

void MapEditorSelection::setMainSelectedSprite(unsigned int mainSelectedSprite) {
	if (selectedSprites.find(mainSelectedSprite) != selectedSprites.end() || mainSelectedSprite == (unsigned int)-1) {
		if (this->mainSelectedSprite != (unsigned int)-1)
			((SpriteObject*)gameManager->getObject(this->mainSelectedSprite))->setSelectionState(1);

		this->mainSelectedSprite = mainSelectedSprite;

		if (mainSelectedSprite != (unsigned int)-1)
			((SpriteObject*)gameManager->getObject(mainSelectedSprite))->setSelectionState(2);

		if (mapObjectProperties) {
			mapObjectProperties->updateProperties();
			loadBox();
		}
	}
	/*else
		printf("Can't set mainSelectedSprite to %u\n", mainSelectedSprite);*/
}

list<unsigned int> MapEditorSelection::getSelectedSprites() {
	list<unsigned int> result;
	for (auto& it : selectedSprites)
		result.push_back(it);
	return result;
}

unsigned int MapEditorSelection::getMainSelectedSprite() {
	return mainSelectedSprite;
}

void MapEditorSelection::selectVertex(unsigned int vertexId, unsigned int layerId) {
	vector<unsigned int> trianglesToUpdate;

	Vec2 pos = sharedTriangleData->getSharedVertexPosition(vertexId);
	list<unsigned int> possibleObjects = gameManager->getObjectsAtPosition(layerId, pos.x, pos.y);
	for (auto& it : possibleObjects) {
		GameObject* possibleObject = gameManager->getObject(it);
		if (possibleObject->getObjectType() == TRIANGLE_OBJECT)
			trianglesToUpdate.push_back(it);
	}
	selectVertex(vertexId, trianglesToUpdate, layerId);
}

void MapEditorSelection::selectVertex(unsigned int vertexId, vector<unsigned int> trianglesToUpdate, unsigned int layerId) {
	if (sharedTriangleData->vertexExists(vertexId)) {
		Renderer* renderer = spriteManager->getRenderer();

		if (selectedVertices.find(vertexId) != selectedVertices.end()) {
			selectedVertices.erase(vertexId);

			for (auto& it : trianglesToUpdate) {
				TriangleObject* cTriangle = (TriangleObject*)gameManager->getObject(it);

				vector<unsigned int> adjacentVertices = cTriangle->getAdjacentVertices(vertexId);
				for (auto& vOther : adjacentVertices) {
					pair<unsigned int, unsigned int> edgeToFind;
					edgeToFind.first = vertexId < vOther ? vertexId : vOther;
					edgeToFind.second = vertexId > vOther ? vertexId : vOther;

					if (selectionEdges.find(edgeToFind) != selectionEdges.end()) {
						SelectionEdgeData& foundEdge = selectionEdges[edgeToFind];
						if (vertexId < vOther) {
							foundEdge.v1Selected = false;
							if (foundEdge.v2Selected == false) {
								renderer->removeColorVertex(foundEdge.v1);
								renderer->removeColorVertex(foundEdge.v2);
								selectionEdges.erase(edgeToFind);
							}
							else
								renderer->setColorVertexColor(foundEdge.v1, 255, 255, 255, 0);
						}
						else {
							foundEdge.v2Selected = false;
							if (foundEdge.v1Selected == false) {
								renderer->removeColorVertex(foundEdge.v1);
								renderer->removeColorVertex(foundEdge.v2);
								selectionEdges.erase(edgeToFind);
							}
							else
								renderer->setColorVertexColor(foundEdge.v2, 255, 255, 255, 0);
						}
					}
				}

				unsigned int v1 = cTriangle->getV1();
				unsigned int v2 = cTriangle->getV2();
				unsigned int v3 = cTriangle->getV3();
				if (selectedVertices.find(v1) == selectedVertices.end() || selectedVertices.find(v2) == selectedVertices.end() || selectedVertices.find(v3) == selectedVertices.end())
					cTriangle->setSelected(false);
				if (selectedVertices.find(v1) == selectedVertices.end() && selectedVertices.find(v2) == selectedVertices.end() && selectedVertices.find(v3) == selectedVertices.end())
					selectedTriangles.erase(it);
			}
		}
		else {
			selectedVertices[vertexId] = layerId;

			for (auto& it : trianglesToUpdate) {
				TriangleObject* cTriangle = (TriangleObject*)gameManager->getObject(it);

				vector<unsigned int> adjacentVertices = cTriangle->getAdjacentVertices(vertexId);
				for (auto& vOther : adjacentVertices) {
					pair<unsigned int, unsigned int> edgeToFind;
					edgeToFind.first = vertexId < vOther ? vertexId : vOther;
					edgeToFind.second = vertexId > vOther ? vertexId : vOther;

					if (selectionEdges.find(edgeToFind) == selectionEdges.end()) {
						SelectionEdgeData& newEdge = selectionEdges[edgeToFind];
						newEdge.overlayLayer = gameManager->getOverlayLayer(layerId);
						Vec2 pos1 = sharedTriangleData->getSharedVertexPosition(edgeToFind.first);
						Vec2 pos2 = sharedTriangleData->getSharedVertexPosition(edgeToFind.second);
						if (vertexId < vOther) {
							newEdge.v1Selected = true;
							newEdge.v2Selected = false;
							newEdge.v1 = renderer->addColorVertex(pos1.x, pos1.y, 255, 255, 255, 255);
							newEdge.v2 = renderer->addColorVertex(pos2.x, pos2.y, 255, 255, 255, 0);
						}
						else {
							newEdge.v1Selected = false;
							newEdge.v2Selected = true;
							newEdge.v1 = renderer->addColorVertex(pos1.x, pos1.y, 255, 255, 255, 0);
							newEdge.v2 = renderer->addColorVertex(pos2.x, pos2.y, 255, 255, 255, 255);
						}
					}
					else {
						SelectionEdgeData& foundEdge = selectionEdges[edgeToFind];
						if (vertexId < vOther) {
							foundEdge.v1Selected = true;
							renderer->setColorVertexColor(foundEdge.v1, 255, 255, 255, 255);
						}
						else {
							foundEdge.v2Selected = true;
							renderer->setColorVertexColor(foundEdge.v2, 255, 255, 255, 255);
						}
					}
				}

				selectedTriangles.insert(it);

				unsigned int v1 = cTriangle->getV1();
				unsigned int v2 = cTriangle->getV2();
				unsigned int v3 = cTriangle->getV3();
				if (selectedVertices.find(v1) != selectedVertices.end() && selectedVertices.find(v2) != selectedVertices.end() && selectedVertices.find(v3) != selectedVertices.end())
					cTriangle->setSelected(true);
			}
		}
	}
	else
		printf("Vertex %u doesn't exist!\n", vertexId);
}

unordered_map<unsigned int, unsigned int> MapEditorSelection::getSelectedVertices() {
	return selectedVertices;
}

void MapEditorSelection::updateSelection() {
	Renderer* renderer = spriteManager->getRenderer();

	for (auto& it : selectedSprites) {
		gameManager->updateObjectAABB(it);
	}

	for (auto& it : selectedTriangles) {
		TriangleObject* cTriangle = (TriangleObject*)gameManager->getObject(it);
		cTriangle->updateVertices();
		gameManager->updateObjectAABB(it);
	}

	for (auto& it : selectionEdges) {
		Vec2 pos = sharedTriangleData->getSharedVertexPosition(it.first.first);
		renderer->setColorVertexPosition(it.second.v1, pos.x, pos.y);
		pos = sharedTriangleData->getSharedVertexPosition(it.first.second);
		renderer->setColorVertexPosition(it.second.v2, pos.x, pos.y);
	}
}

void MapEditorSelection::flagForRender() {
	for (auto& it : selectionEdges)
		it.second.overlayLayer->addLine(it.second.v1, it.second.v2);
}

GameManager* MapEditorSelection::getGameManager() {
	return gameManager;
}

SharedTriangleData* MapEditorSelection::getSharedTriangleData() {
	return sharedTriangleData;
}
