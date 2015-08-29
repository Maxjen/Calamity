#pragma once

#include <string>
#include <utility>
#include <vector>

#include "GameManager.h"
#include "Math/Vec2.h"
#include "SharedTriangleData.h"

#include "Box2D/Collision/b2DynamicTree.h"

using std::string;
using std::pair;
using std::vector;

using namespace clm;

class TriangleObject : public GameObject {
private:
	RenderLayer* renderLayer;
	RenderLayer* overlayLayer;

	SharedTriangleData* sharedTriangleData;
	unsigned int v1Shared, v2Shared, v3Shared;

	unsigned int v1, v2, v3;
	string textureName;
	unsigned int textureId;
	float textureWidth, textureHeight;

	//int state;
	bool bSelected;
	unsigned int v1Selection, v2Selection, v3Selection;
public:
	explicit TriangleObject(SharedTriangleData* sharedTriangleData, unsigned int v1Shared, unsigned int v2Shared, unsigned int v3Shared, const char* textureName);
	void setRenderLayers(RenderLayer* renderLayer, RenderLayer* overlayLayer);
	b2AABB getAABB();
	void remove();
	void restore();
	int getObjectType();
	bool containsPoint(float x, float y);
	void flagForRender();

	bool containsVertex(unsigned int v);

	unsigned int getV1();
	unsigned int getV2();
	unsigned int getV3();

	pair<unsigned int, float> getClosestVertex(float x, float y);
	vector<unsigned int> getAdjacentVertices(unsigned int v);

	//void setVertexId(unsigned int index, unsigned int vertexId);

	//void toggleSelection();
	//void setSelectionState(int newState);
	void setSelected(bool bSelected);
	void updateVertices();
	void updateSelection();
};
