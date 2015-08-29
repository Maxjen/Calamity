#include "TriangleObject.h"

TriangleObject::TriangleObject(SharedTriangleData* sharedTriangleData, unsigned int v1Shared, unsigned int v2Shared, unsigned int v3Shared, const char* textureName) :
	renderLayer(nullptr),
	overlayLayer(nullptr),
	sharedTriangleData(sharedTriangleData),
	v1Shared(v1Shared),
	v2Shared(v2Shared),
	v3Shared(v3Shared),
	v1(-1),
	v2(-1),
	v3(-1),
	textureName(textureName),
	textureId(-1),

	bSelected(false),
	v1Selection(-1),
	v2Selection(-1),
	v3Selection(-1)
{}

void TriangleObject::setRenderLayers(RenderLayer* renderLayer, RenderLayer* overlayLayer) {
	this->renderLayer = renderLayer;
	this->overlayLayer = overlayLayer;

	Renderer* renderer = renderLayer->getRenderer();

	textureId = renderer->getTextureManager()->createTexture(textureName.c_str());
	textureWidth = renderer->getTextureManager()->getTextureWidth(textureId);
	textureHeight = renderer->getTextureManager()->getTextureHeight(textureId);

	Vec2 pos;
	float u, v;
	char r, g, b, a;

	pos = sharedTriangleData->getSharedVertexPosition(v1Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v1Shared);
	g = sharedTriangleData->getSharedVertexG(v1Shared);
	b = sharedTriangleData->getSharedVertexB(v1Shared);
	a = sharedTriangleData->getSharedVertexA(v1Shared);
	v1 = renderer->addTextureVertex(pos.x, pos.y, u, v, r, g, b, a, textureId);

	pos = sharedTriangleData->getSharedVertexPosition(v2Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v2Shared);
	g = sharedTriangleData->getSharedVertexG(v2Shared);
	b = sharedTriangleData->getSharedVertexB(v2Shared);
	a = sharedTriangleData->getSharedVertexA(v2Shared);
	v2 = renderer->addTextureVertex(pos.x, pos.y, u, v, r, g, b, a, textureId);

	pos = sharedTriangleData->getSharedVertexPosition(v3Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v3Shared);
	g = sharedTriangleData->getSharedVertexG(v3Shared);
	b = sharedTriangleData->getSharedVertexB(v3Shared);
	a = sharedTriangleData->getSharedVertexA(v3Shared);
	v3 = renderer->addTextureVertex(pos.x, pos.y, u, v, r, g, b, a, textureId);
}

b2AABB TriangleObject::getAABB() {
	Vec2 v1Pos = sharedTriangleData->getSharedVertexPosition(v1Shared);
	Vec2 v2Pos = sharedTriangleData->getSharedVertexPosition(v2Shared);
	Vec2 v3Pos = sharedTriangleData->getSharedVertexPosition(v3Shared);


	float xMin = std::min(v1Pos.x, v2Pos.x);
	xMin = std::min(xMin, v3Pos.x);
	float xMax = std::max(v1Pos.x, v2Pos.x);
	xMax = std::max(xMax, v3Pos.x);
	float yMin = std::min(v1Pos.y, v2Pos.y);
	yMin = std::min(yMin, v3Pos.y);
	float yMax = std::max(v1Pos.y, v2Pos.y);
	yMax = std::max(yMax, v3Pos.y);

	b2AABB aabb;
	aabb.lowerBound.x = xMin - 20;
	aabb.lowerBound.y = yMin - 20;
	aabb.upperBound.x = xMax + 20;
	aabb.upperBound.y = yMax + 20;

	return aabb;
}

void TriangleObject::remove() {
	/*Renderer* renderer = renderLayer->getRenderer();

	if (state != 0) {
		renderer->removeColorVertex(topLeft);
		renderer->removeColorVertex(bottomLeft);
		renderer->removeColorVertex(bottomRight);
		renderer->removeColorVertex(topRight);
	}*/

	Renderer* renderer = renderLayer->getRenderer();

	if (v1 != (unsigned int)-1) {
		renderer->removeTextureVertex(v1);
		renderer->removeTextureVertex(v2);
		renderer->removeTextureVertex(v3);

		v1 = (unsigned int)-1;
		v2 = (unsigned int)-1;
		v3 = (unsigned int)-1;

		renderer->getTextureManager()->deleteTexture(textureId);
	}
}

void TriangleObject::restore() {
	/*if (state) {
		int stateTmp = state;
		state = 0;
		setSelectionState(stateTmp);
	}

	updateSelection();*/

	Renderer* renderer = renderLayer->getRenderer();

	textureId = renderer->getTextureManager()->createTexture(textureName.c_str());
	textureWidth = renderer->getTextureManager()->getTextureWidth(textureId);
	textureHeight = renderer->getTextureManager()->getTextureHeight(textureId);

	Vec2 pos;
	float u, v;
	char r, g, b, a;

	pos = sharedTriangleData->getSharedVertexPosition(v1Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v1Shared);
	g = sharedTriangleData->getSharedVertexG(v1Shared);
	b = sharedTriangleData->getSharedVertexB(v1Shared);
	a = sharedTriangleData->getSharedVertexA(v1Shared);
	v1 = renderer->addTextureVertex(pos.x, pos.y, u, v, r, g, b, a, textureId);

	pos = sharedTriangleData->getSharedVertexPosition(v2Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v2Shared);
	g = sharedTriangleData->getSharedVertexG(v2Shared);
	b = sharedTriangleData->getSharedVertexB(v2Shared);
	a = sharedTriangleData->getSharedVertexA(v2Shared);
	v2 = renderer->addTextureVertex(pos.x, pos.y, u, v, r, g, b, a, textureId);

	pos = sharedTriangleData->getSharedVertexPosition(v3Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v3Shared);
	g = sharedTriangleData->getSharedVertexG(v3Shared);
	b = sharedTriangleData->getSharedVertexB(v3Shared);
	a = sharedTriangleData->getSharedVertexA(v3Shared);
	v3 = renderer->addTextureVertex(pos.x, pos.y, u, v, r, g, b, a, textureId);
}

int TriangleObject::getObjectType() {
	return TRIANGLE_OBJECT;
}

bool TriangleObject::containsPoint(float x, float y) {
	return true;
}

void TriangleObject::flagForRender() {
	if (renderLayer)
		renderLayer->addTextureTriangle(v1, v2, v3);
	else
		printf("No renderLayer!");

	//if (isSelected) {
	/*if (state) {
		overlayLayer->addLine(topLeft, bottomLeft);
		overlayLayer->addLine(bottomLeft, bottomRight);
		overlayLayer->addLine(bottomRight, topRight);
		overlayLayer->addLine(topRight, topLeft);
	}*/
	if (bSelected) {
		overlayLayer->addTriangle(v1Selection, v2Selection, v3Selection);
	}
}

bool TriangleObject::containsVertex(unsigned int v) {
	return (v1Shared == v || v2Shared == v || v3Shared == v);
}

unsigned int TriangleObject::getV1() {
	return v1Shared;
}

unsigned int TriangleObject::getV2() {
	return v2Shared;
}

unsigned int TriangleObject::getV3() {
	return v3Shared;
}

pair<unsigned int, float> TriangleObject::getClosestVertex(float x, float y) {
	pair<unsigned int, float> result(-1, 400.0f);

	Vec2 pos;
	float dx, dy, distanceSq;

	pos = sharedTriangleData->getSharedVertexPosition(v1Shared);
	dx = pos.x - x;
	dy = pos.y - y;
	distanceSq = dx * dx + dy * dy;
	if (distanceSq < result.second) {
		result.first = v1Shared;
		result.second = distanceSq;
	}

	pos = sharedTriangleData->getSharedVertexPosition(v2Shared);
	dx = pos.x - x;
	dy = pos.y - y;
	distanceSq = dx * dx + dy * dy;
	if (distanceSq < result.second) {
		result.first = v2Shared;
		result.second = distanceSq;
	}

	pos = sharedTriangleData->getSharedVertexPosition(v3Shared);
	dx = pos.x - x;
	dy = pos.y - y;
	distanceSq = dx * dx + dy * dy;
	if (distanceSq < result.second) {
		result.first = v3Shared;
		result.second = distanceSq;
	}

	return result;
}

vector<unsigned int> TriangleObject::getAdjacentVertices(unsigned int v) {
	vector<unsigned int> result;

	if (v == v1Shared) {
		result.push_back(v2Shared);
		result.push_back(v3Shared);
	}
	else if (v == v2Shared) {
		result.push_back(v1Shared);
		result.push_back(v3Shared);
	}
	else if (v == v3Shared) {
		result.push_back(v1Shared);
		result.push_back(v2Shared);
	}
	/*else
		printf("Triangle has no sharedVertex with id %u\n", v);*/

	return result;
}

/*void TriangleObject::setVertexId(unsigned int index, unsigned int vertexId) {
	if (index == 0)
		v1 = vertexId;
	else if (index == 1)
		v2 = vertexId;
	else if (index == 2)
		v3 = vertexId;
	else
		printf("Index not in range!\n");
}*/

/*void TriangleObject::setSelectionState(int newState) {
	Renderer* renderer = renderLayer->getRenderer();

	if (newState == 0 && state != 0) {
		renderer->removeColorVertex(topLeft);
		renderer->removeColorVertex(bottomLeft);
		renderer->removeColorVertex(bottomRight);
		renderer->removeColorVertex(topRight);
	}
	else if (newState == 1) {
		if (state == 0) {
			Vec2 pos;
			pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
			topLeft = renderer->addColorVertex(pos.x, pos.y, 155, 155, 155, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
			bottomLeft = renderer->addColorVertex(pos.x, pos.y, 155, 155, 155, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
			bottomRight = renderer->addColorVertex(pos.x, pos.y, 155, 155, 155, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
			topRight = renderer->addColorVertex(pos.x, pos.y, 155, 155, 155, 255);
		}
		else if (state == 2) {
			renderer->setColorVertexColor(topLeft, 155, 155, 155, 255);
			renderer->setColorVertexColor(bottomLeft, 155, 155, 155, 255);
			renderer->setColorVertexColor(bottomRight, 155, 155, 155, 255);
			renderer->setColorVertexColor(topRight, 155, 155, 155, 255);
		}
	}
	else if (newState == 2) {
		if (state == 0) {
			Vec2 pos;
			pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
			topLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
			bottomLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
			bottomRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
			pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
			topRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		}
		else if (state == 1) {
			renderer->setColorVertexColor(topLeft, 255, 255, 255, 255);
			renderer->setColorVertexColor(bottomLeft, 255, 255, 255, 255);
			renderer->setColorVertexColor(bottomRight, 255, 255, 255, 255);
			renderer->setColorVertexColor(topRight, 255, 255, 255, 255);
		}
	}
	else {
		return;
	}
	state = newState;
}*/

/*void TriangleObject::toggleSelection() {
	Renderer* renderer = renderLayer->getRenderer();
	if (isSelected) {
		renderer->removeColorVertex(topLeft);
		renderer->removeColorVertex(bottomLeft);
		renderer->removeColorVertex(bottomRight);
		renderer->removeColorVertex(topRight);
		isSelected = false;
	}
	else {
		Vec2 pos;
		pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
		topLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
		bottomLeft = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
		bottomRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
		topRight = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 255);
		isSelected = true;
	}
}*/

void TriangleObject::setSelected(bool bSelected) {
	Renderer* renderer = renderLayer->getRenderer();

	if (this->bSelected && !bSelected) {
		renderer->removeColorVertex(v1Selection);
		renderer->removeColorVertex(v2Selection);
		renderer->removeColorVertex(v3Selection);
	}
	else if (!this->bSelected && bSelected) {
		Vec2 pos;

		pos = sharedTriangleData->getSharedVertexPosition(v1Shared);
		v1Selection = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 30);

		pos = sharedTriangleData->getSharedVertexPosition(v2Shared);
		v2Selection = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 30);

		pos = sharedTriangleData->getSharedVertexPosition(v3Shared);
		v3Selection = renderer->addColorVertex(pos.x, pos.y, 255, 255, 255, 30);
	}
	this->bSelected = bSelected;
}

void TriangleObject::updateVertices() {
	Renderer* renderer = renderLayer->getRenderer();

	Vec2 pos;
	float u, v;
	char r, g, b, a;

	pos = sharedTriangleData->getSharedVertexPosition(v1Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v1Shared);
	g = sharedTriangleData->getSharedVertexG(v1Shared);
	b = sharedTriangleData->getSharedVertexB(v1Shared);
	a = sharedTriangleData->getSharedVertexA(v1Shared);
	renderer->setTextureVertexPosition(v1, pos.x, pos.y);
	renderer->setTextureVertexUV(v1, u, v);
	renderer->setTextureVertexColor(v1, r, g, b, a);
	if (bSelected)
		renderer->setColorVertexPosition(v1Selection, pos.x, pos.y);

	pos = sharedTriangleData->getSharedVertexPosition(v2Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v2Shared);
	g = sharedTriangleData->getSharedVertexG(v2Shared);
	b = sharedTriangleData->getSharedVertexB(v2Shared);
	a = sharedTriangleData->getSharedVertexA(v2Shared);
	renderer->setTextureVertexPosition(v2, pos.x, pos.y);
	renderer->setTextureVertexUV(v2, u, v);
	renderer->setTextureVertexColor(v2, r, g, b, a);
	if (bSelected)
		renderer->setColorVertexPosition(v2Selection, pos.x, pos.y);

	pos = sharedTriangleData->getSharedVertexPosition(v3Shared);
	u = pos.x / textureWidth;
	v = pos.y / textureHeight;
	r = sharedTriangleData->getSharedVertexR(v3Shared);
	g = sharedTriangleData->getSharedVertexG(v3Shared);
	b = sharedTriangleData->getSharedVertexB(v3Shared);
	a = sharedTriangleData->getSharedVertexA(v3Shared);
	renderer->setTextureVertexPosition(v3, pos.x, pos.y);
	renderer->setTextureVertexUV(v3, u, v);
	renderer->setTextureVertexColor(v3, r, g, b, a);
	if (bSelected)
		renderer->setColorVertexPosition(v3Selection, pos.x, pos.y);
}

void TriangleObject::updateSelection() {
	//if (isSelected) {
	/*if (state) {
		Renderer* renderer = renderLayer->getRenderer();
		Vec2 pos;
		pos = spriteManager->getSpriteVertexPosition(spriteId, 0);
		renderer->setColorVertexPosition(topLeft, pos.x, pos.y);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 1);
		renderer->setColorVertexPosition(bottomLeft, pos.x, pos.y);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 2);
		renderer->setColorVertexPosition(bottomRight, pos.x, pos.y);
		pos = spriteManager->getSpriteVertexPosition(spriteId, 3);
		renderer->setColorVertexPosition(topRight, pos.x, pos.y);
	}*/
}
