#pragma once

#include <set>
#include <map>
#include <unordered_map>
#include <utility>

#include "SpriteManager.h"

#include "../Common/GameManager.h"
#include "../Common/TriangleObject.h"
#include "../Common/SpriteObject.h"

#include "MapObjectProperties.h"
#include "Gui/VerticalBoxLayout.h"
#include "Gui/InputGroup.h"

using std::set;
using std::map;
using std::unordered_map;

using namespace clm;

class MapObjectProperties;

/*class TriangleSelection {
public:
	unordered_map<unsigned int, unsigned int> selectedTriangles;
};*/

class MapEditorSelection {
private:
	struct SelectionEdgeData {
		bool v1Selected, v2Selected;
		unsigned int v1, v2;
		RenderLayer* overlayLayer;
	};

	GameManager* gameManager;
	SpriteManager* spriteManager;
	SharedTriangleData* sharedTriangleData;

	set<unsigned int> selectedSprites;
	unsigned int mainSelectedSprite;

	// maps sharedVertexId to layerId
	unordered_map<unsigned int, unsigned int> selectedVertices;
	set<unsigned int> selectedTriangles;

	// maps sharedVertexIds to colorVertexIds
	map<pair<unsigned int, unsigned int>, SelectionEdgeData> selectionEdges;

	MapObjectProperties* mapObjectProperties;

	VerticalBoxLayout* boxLayout;
	unsigned int boxId;
	unsigned int fontId;
public:
	explicit MapEditorSelection(GameManager* gameManager, SpriteManager* spriteManager, SharedTriangleData* sharedTriangleData);
	void setMapObjectProperties(MapObjectProperties* mapObjectProperties);
	void updateProperties();
	void setBoxWidget(VerticalBoxLayout* boxLayout, unsigned int boxId, unsigned int fontId);
	void loadBox();

	void selectSprite(unsigned int spriteId);
	void setMainSelectedSprite(unsigned int mainSelectedSprite);
	list<unsigned int> getSelectedSprites();
	unsigned int getMainSelectedSprite();

	void selectVertex(unsigned int vertexId, unsigned int layerId);
	void selectVertex(unsigned int vertexId, vector<unsigned int> trianglesToUpdate, unsigned int layerId);
	unordered_map<unsigned int, unsigned int> getSelectedVertices();

	void updateSelection();

	void flagForRender();
	GameManager* getGameManager();
	SharedTriangleData* getSharedTriangleData();
};
