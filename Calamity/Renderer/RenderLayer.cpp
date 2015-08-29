#include "RenderLayer.h"

namespace clm {

RenderLayer::RenderLayer(Renderer* renderer) :
	renderer(renderer),
	layers()
{}

RenderLayer::~RenderLayer() {
	for (auto& it : layers) {
		if (it.layerType == SPRITE_LAYER)
			delete it.layerPointer.spriteLayer;
		if (it.layerType == TEXTURE_TRIANGLE_LAYER)
			delete it.layerPointer.textureTriangleLayer;
		if (it.layerType == FONT_LAYER)
			delete it.layerPointer.fontLayer;
		if (it.layerType == LINE_LAYER)
			delete it.layerPointer.lineLayer;
		if (it.layerType == TRIANGLE_LAYER)
			delete it.layerPointer.triangleLayer;
	}
}

Renderer* RenderLayer::getRenderer() {
	return renderer;
}

/*int RenderLayer::getSpriteLayer() {
	if (!layers.empty() && layers.back().layerType == SPRITELAYER)
		return layers.back().layerId;
	else {
		Layer newLayer;
		newLayer.layerType = SPRITELAYER;
		newLayer.layerId = spriteRenderer->addSpriteLayer();
		layers.push_back(newLayer);
		return layers.back().layerId;
	}
}*/

void RenderLayer::addSpriteTriangle(unsigned int v1, unsigned int v2, unsigned int v3) {
	if (layers.empty() || layers.back().layerType != SPRITE_LAYER) {
		Layer newLayer;
		newLayer.layerType = SPRITE_LAYER;
		newLayer.layerPointer.spriteLayer = new SpriteLayer();
		renderer->initSpriteLayer(newLayer.layerPointer.spriteLayer);
		layers.push_back(newLayer);
	}
	SpriteLayer* spriteLayer = layers.back().layerPointer.spriteLayer;
	renderer->addSpriteTriangleToLayer(spriteLayer, v1, v2, v3);
}

void RenderLayer::addTextureTriangle(unsigned int v1, unsigned int v2, unsigned int v3) {
	if (layers.empty() || layers.back().layerType != TEXTURE_TRIANGLE_LAYER) {
		Layer newLayer;
		newLayer.layerType = TEXTURE_TRIANGLE_LAYER;
		newLayer.layerPointer.textureTriangleLayer = new TextureTriangleLayer();
		renderer->initTextureTriangleLayer(newLayer.layerPointer.textureTriangleLayer);
		layers.push_back(newLayer);
	}
	TextureTriangleLayer* textureTriangleLayer = layers.back().layerPointer.textureTriangleLayer;
	renderer->addTextureTriangleToLayer(textureTriangleLayer, v1, v2, v3);
}

void RenderLayer::addLetter(wchar_t letter, unsigned int fontId, float x, float y, char r, char g, char b, char a) {
	if (layers.empty() || layers.back().layerType != FONT_LAYER) {
		Layer newLayer;
		newLayer.layerType = FONT_LAYER;
		newLayer.layerPointer.fontLayer = new FontLayer();
		renderer->initFontLayer(newLayer.layerPointer.fontLayer);
		layers.push_back(newLayer);
	}
	FontLayer* fontLayer = layers.back().layerPointer.fontLayer;
	renderer->addLetterToLayer(fontLayer, letter, x, y, r, g, b, a, fontId);
}

void RenderLayer::addLine(unsigned int v1, unsigned int v2) {
	if (layers.empty() || layers.back().layerType != LINE_LAYER) {
		Layer newLayer;
		newLayer.layerType = LINE_LAYER;
		newLayer.layerPointer.lineLayer = new LineLayer();
		renderer->initLineLayer(newLayer.layerPointer.lineLayer);
		layers.push_back(newLayer);
	}
	LineLayer* lineLayer = layers.back().layerPointer.lineLayer;
	renderer->addLineToLayer(lineLayer, v1, v2);
}

void RenderLayer::addTriangle(unsigned int v1, unsigned int v2, unsigned int v3) {
	if (layers.empty() || layers.back().layerType != TRIANGLE_LAYER) {
		Layer newLayer;
		newLayer.layerType = TRIANGLE_LAYER;
		newLayer.layerPointer.triangleLayer = new TriangleLayer();
		renderer->initTriangleLayer(newLayer.layerPointer.triangleLayer);
		layers.push_back(newLayer);
	}
	TriangleLayer* triangleLayer = layers.back().layerPointer.triangleLayer;
	renderer->addTriangleToLayer(triangleLayer, v1, v2, v3);
}

void RenderLayer::uploadLayerBuffers() {
	for (auto& it : layers) {
		if (it.layerType == SPRITE_LAYER)
			it.layerPointer.spriteLayer->uploadBuffers();
		if (it.layerType == TEXTURE_TRIANGLE_LAYER)
			it.layerPointer.textureTriangleLayer->uploadBuffers();
		if (it.layerType == FONT_LAYER)
			it.layerPointer.fontLayer->uploadBuffers();
		if (it.layerType == LINE_LAYER)
			it.layerPointer.lineLayer->uploadBuffers();
		if (it.layerType == TRIANGLE_LAYER)
			it.layerPointer.triangleLayer->uploadBuffers();
	}
}

void RenderLayer::clearLayers() {
	for (auto& it : layers) {
		if (it.layerType == SPRITE_LAYER)
			delete it.layerPointer.spriteLayer;
		if (it.layerType == TEXTURE_TRIANGLE_LAYER)
			delete it.layerPointer.textureTriangleLayer;
		if (it.layerType == FONT_LAYER)
			delete it.layerPointer.fontLayer;
		if (it.layerType == LINE_LAYER)
			delete it.layerPointer.lineLayer;
		if (it.layerType == TRIANGLE_LAYER)
			delete it.layerPointer.triangleLayer;
	}
	layers.clear();
}

void RenderLayer::draw() {
	for (auto& it : layers) {
		if (it.layerType == SPRITE_LAYER)
			renderer->drawSpriteLayer(it.layerPointer.spriteLayer);
		if (it.layerType == TEXTURE_TRIANGLE_LAYER)
			renderer->drawTextureTriangleLayer(it.layerPointer.textureTriangleLayer);
		if (it.layerType == FONT_LAYER)
			renderer->drawFontLayer(it.layerPointer.fontLayer);
		if (it.layerType == LINE_LAYER)
			renderer->drawLineLayer(it.layerPointer.lineLayer);
		if (it.layerType == TRIANGLE_LAYER)
			renderer->drawTriangleLayer(it.layerPointer.triangleLayer);
	}
}

}
