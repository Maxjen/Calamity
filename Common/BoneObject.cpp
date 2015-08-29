#include "BoneObject.h"

BoneObject::BoneObject(SpriteManager* spriteManager, float x1, float y1, float x2, float y2) :
	spriteManager(spriteManager),
	renderLayer(nullptr),
	overlayLayer(nullptr),

	isMainSelectedObject(false),
	rootSelected(false),
	tipSelected(false),

	parent(nullptr),
	children(),

	x1(x1),
	y1(y1),
	x2(x2),
	y2(y2),

	angle(atan2(y2 - y1, x2 - x1)),
	length(distance(Vec2(x1, y1), Vec2(x2, y2))),

	root(-1),
	tip(-1),
	left(-1),
	right(-1),

	rootT(-1),
	tipT(-1),
	leftT(-1),
	rightT(-1),

	lineToParentStart(-1),
	lineToParentEnd(-1)
{
	addBoneOverlay();
}

void BoneObject::setRenderLayers(RenderLayer* renderLayer, RenderLayer* overlayLayer) {
	this->renderLayer = renderLayer;
	this->overlayLayer = overlayLayer;
}

b2AABB BoneObject::getAABB() {
	// TODO: combine sprites and bone
	b2AABB aabb;
	aabb.lowerBound.x = std::min(x1, x2) - 10.0f;
	aabb.lowerBound.y = std::min(y1, y2) - 10.0f;
	aabb.upperBound.x = std::max(x1, x2) + 10.0f;
	aabb.upperBound.y = std::max(y1, y2) + 10.0f;
	return aabb;
}

void BoneObject::remove() {
	Renderer* renderer = spriteManager->getRenderer();

	renderer->removeColorVertex(root);
	renderer->removeColorVertex(left);
	renderer->removeColorVertex(right);
	renderer->removeColorVertex(tip);

	renderer->removeColorVertex(rootT);
	renderer->removeColorVertex(leftT);
	renderer->removeColorVertex(rightT);
	renderer->removeColorVertex(tipT);

	if (parent) {
		parent->removeChild(this);

		renderer->removeColorVertex(lineToParentStart);
		renderer->removeColorVertex(lineToParentEnd);
	}


	for (auto& it : children)
		it->setParent(nullptr);
}

void BoneObject::restore() {
	addBoneOverlay();

	if (parent) {
		parent->addChild(this);

		Renderer* renderer = spriteManager->getRenderer();
		Vec2 parentTip = parent->getTipPosition();
		this->lineToParentStart = renderer->addColorVertex(x1, y1, 100, 100, 100, 100);
		this->lineToParentEnd = renderer->addColorVertex(parentTip.x, parentTip.y, 100, 100, 100, 100);
	}

	for (auto& it : children)
		it->setParent(this);
}

int BoneObject::getObjectType() {
	return BONE_OBJECT;
}

void BoneObject::flagForRender() {
	overlayLayer->addLine(root, left);
	overlayLayer->addLine(left, tip);
	overlayLayer->addLine(root, right);
	overlayLayer->addLine(right, tip);

	overlayLayer->addTriangle(rootT, rightT, leftT);
	overlayLayer->addTriangle(leftT, rightT, tipT);

	if (parent)
		overlayLayer->addLine(lineToParentStart, lineToParentEnd);

	/*spriteManager->flagSpriteForRender(spriteId);

	if (isSelected) {
		overlayLayer->addLine(topLeft, bottomLeft);
		overlayLayer->addLine(bottomLeft, bottomRight);
		overlayLayer->addLine(bottomRight, topRight);
		overlayLayer->addLine(topRight, topLeft);
	}*/
}

void BoneObject::addBoneOverlay() {
	Vec2 root(x1, y1);
	Vec2 left, right;
	if (length >= 20.0f) {
		left = vectorRot2D(root + Vec2(10.0f, 2.0f), root, angle);
		right = vectorRot2D(root + Vec2(10.0f, -2.0f), root, angle);
	}
	else {
		left = vectorRot2D(root + Vec2(length/2, 2.0f), root, angle);
		right = vectorRot2D(root + Vec2(length/2, -2.0f), root, angle);
	}

	Renderer* renderer = spriteManager->getRenderer();
	this->root = renderer->addColorVertex(x1, y1, 100, 100, 100, 255);
	this->left = renderer->addColorVertex(left.x, left.y, 100, 100, 100, 255);
	this->right = renderer->addColorVertex(right.x, right.y, 100, 100, 100, 255);
	this->tip = renderer->addColorVertex(x2, y2, 100, 100, 100, 255);

	this->rootT = renderer->addColorVertex(x1, y1, 100, 100, 100, 100);
	this->leftT = renderer->addColorVertex(left.x, left.y, 100, 100, 100, 100);
	this->rightT = renderer->addColorVertex(right.x, right.y, 100, 100, 100, 100);
	this->tipT = renderer->addColorVertex(x2, y2, 100, 100, 100, 100);
}

void BoneObject::updateBoneOverlay() {
	Vec2 root(x1, y1);
	Vec2 left, right;
	if (length >= 20.0f) {
		left = vectorRot2D(root + Vec2(10.0f, 2.0f), root, angle);
		right = vectorRot2D(root + Vec2(10.0f, -2.0f), root, angle);
	}
	else {
		left = vectorRot2D(root + Vec2(length/2, 2.0f), root, angle);
		right = vectorRot2D(root + Vec2(length/2, -2.0f), root, angle);
	}

	Renderer* renderer = spriteManager->getRenderer();
	renderer->setColorVertexPosition(this->root, x1, y1);
	renderer->setColorVertexPosition(this->left, left.x, left.y);
	renderer->setColorVertexPosition(this->right, right.x, right.y);
	renderer->setColorVertexPosition(this->tip, x2, y2);

	renderer->setColorVertexPosition(this->rootT, x1, y1);
	renderer->setColorVertexPosition(this->leftT, left.x, left.y);
	renderer->setColorVertexPosition(this->rightT, right.x, right.y);
	renderer->setColorVertexPosition(this->tipT, x2, y2);

	if (parent) {
		Vec2 parentTip = parent->getTipPosition();
		renderer->setColorVertexPosition(this->lineToParentStart, x1, y1);
		renderer->setColorVertexPosition(this->lineToParentEnd, parentTip.x, parentTip.y);
	}

	for (auto& it : children)
		it->updateBoneOverlay();
}

BoneObject* BoneObject::getParent() {
	return parent;
}

void BoneObject::setParent(BoneObject* newParent) {
	Renderer* renderer = spriteManager->getRenderer();

	if (parent)
		parent->removeChild(this);

	if (newParent) {
		Vec2 parentTip = newParent->getTipPosition();
		this->lineToParentStart = renderer->addColorVertex(x1, y1, 100, 100, 100, 100);
		this->lineToParentEnd = renderer->addColorVertex(parentTip.x, parentTip.y, 100, 100, 100, 100);

		newParent->addChild(this);
	}
	else {
		renderer->removeColorVertex(lineToParentStart);
		renderer->removeColorVertex(lineToParentEnd);
	}

	parent = newParent;
}

void BoneObject::addChild(BoneObject* child) {
	children.insert(child);
}

void BoneObject::removeChild(BoneObject* child) {
	children.erase(child);
}

Vec2 BoneObject::getRootPosition() {
	return Vec2(x1, y1);
}

void BoneObject::setRootPosition(float x, float y) {
	x1 = x;
	y1 = y;
	angle = atan2(y2 - y1, x2 - x1);
	length = distance(Vec2(x1, y1), Vec2(x2, y2));
	updateBoneOverlay();
}

Vec2 BoneObject::getTipPosition() {
	return Vec2(x2, y2);
}

void BoneObject::setTipPosition(float x, float y) {
	x2 = x;
	y2 = y;
	angle = atan2(y2 - y1, x2 - x1);
	length = distance(Vec2(x1, y1), Vec2(x2, y2));
	updateBoneOverlay();
}

BoneVertexDistance BoneObject::getClosestBoneVertex(float x, float y) {
	BoneVertexDistance result;
	float dist1 = vectorMagP2(Vec2(x, y) - Vec2(x1, y1));
	float dist2 = vectorMagP2(Vec2(x, y) - Vec2(x2, y2));
	if (dist1 == dist2) {
		result.boneVertex = 3;
		result.distance = dist1;
	}
	else {
		result.distance = std::min(dist1, dist2);
		result.boneVertex = (dist1 < dist2) ? 1 : 2;
	}
	return result;
}

/*float BoneObject::getScale() {
	return scale;
}

void BoneObject::setScale(float scale) {
	this->scale = scale;
	spriteManager->setSpriteScale(spriteId, scale);
	updateSelection();
}

float BoneObject::getRotation() {
	return rotation;
}

void BoneObject::setRotation(float rotation) {
	this->rotation = rotation;
	spriteManager->setSpriteRotation(spriteId, rotation);
	updateSelection();
}*/

/*bool BoneObject::containsPoint(float x, float y) {
	return true;
}*/

void BoneObject::toggleRootSelection() {
	rootSelected = !rootSelected;
	/*Renderer* renderer = renderLayer->getRenderer();
	if (rootSelected) {
		renderer->setColorVertexColor(root, 100, 100, 100, 255);
		renderer->setColorVertexColor(left, 100, 100, 100, 255);
		renderer->setColorVertexColor(right, 100, 100, 100, 255);

		renderer->setColorVertexColor(rootT, 100, 100, 100, 100);
		renderer->setColorVertexColor(leftT, 100, 100, 100, 100);
		renderer->setColorVertexColor(rightT, 100, 100, 100, 100);
		rootSelected = false;

	}
	else {
		renderer->setColorVertexColor(root, 255, 255, 255, 255);
		renderer->setColorVertexColor(left, 255, 255, 255, 255);
		renderer->setColorVertexColor(right, 255, 255, 255, 255);

		renderer->setColorVertexColor(rootT, 255, 255, 255, 100);
		renderer->setColorVertexColor(leftT, 255, 255, 255, 100);
		renderer->setColorVertexColor(rightT, 255, 255, 255, 100);
		rootSelected = true;
	}*/
}

void BoneObject::toggleTipSelection() {
	tipSelected = !tipSelected;
	/*Renderer* renderer = renderLayer->getRenderer();
	if (tipSelected) {
		renderer->setColorVertexColor(tip, 100, 100, 100, 255);

		renderer->setColorVertexColor(tipT, 100, 100, 100, 100);
		tipSelected = false;
	}
	else {
		renderer->setColorVertexColor(tip, 255, 255, 255, 255);

		renderer->setColorVertexColor(tipT, 255, 255, 255, 100);
		tipSelected = true;
	}*/
}

void BoneObject::setMainSelectedObject(bool isMainSelectedObject) {
	this->isMainSelectedObject = isMainSelectedObject;
}

void BoneObject::updateBoneOverlayColor() {
	char uR, uG, uB;
	char sR, sG, sB;
	if (isMainSelectedObject) {
		uR = 255;
		uG = 50;
		uB = 0;
		sR = 255;
		sG = 200;
		sB = 0;
	}
	else {
		uR = uG = uB = 100;
		sR = sG = sB = 255;
	}

	Renderer* renderer = renderLayer->getRenderer();

	if (rootSelected) {
		renderer->setColorVertexColor(root, sR, sG, sB, 255);
		renderer->setColorVertexColor(left, sR, sG, sB, 255);
		renderer->setColorVertexColor(right, sR, sG, sB, 255);

		renderer->setColorVertexColor(rootT, sR, sG, sB, 100);
		renderer->setColorVertexColor(leftT, sR, sG, sB, 100);
		renderer->setColorVertexColor(rightT, sR, sG, sB, 100);
	}
	else {
		renderer->setColorVertexColor(root, uR, uG, uB, 255);
		renderer->setColorVertexColor(left, uR, uG, uB, 255);
		renderer->setColorVertexColor(right, uR, uG, uB, 255);

		renderer->setColorVertexColor(rootT, uR, uG, uB, 100);
		renderer->setColorVertexColor(leftT, uR, uG, uB, 100);
		renderer->setColorVertexColor(rightT, uR, uG, uB, 100);
	}

	if (tipSelected) {
		renderer->setColorVertexColor(tip, sR, sG, sB, 255);

		renderer->setColorVertexColor(tipT, sR, sG, sB, 100);
	}
	else {
		renderer->setColorVertexColor(tip, uR, uG, uB, 255);

		renderer->setColorVertexColor(tipT, uR, uG, uB, 100);
	}
}

/*void BoneObject::updateSelection() {
	if (isSelected) {
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
	}
}*/
