#include "RenderLayer.h"

namespace clm {

RenderLayer::RenderLayer(SpriteRenderer* spriteRenderer) {
	this->spriteRenderer = spriteRenderer;
}

RenderLayer::~RenderLayer() {
	for (auto& it : layers) {
		if (it.layerType == SPRITELAYER) {
			spriteRenderer->removeSpriteLayer(it.layerId);
			glDeleteVertexArrays(1, &(it.layerPointer.spriteLayer->vaoId));
			glDeleteBuffers(1, &(it.layerPointer.spriteLayer->vertexBufferId));
			glDeleteBuffers(1, &(it.layerPointer.spriteLayer->indexBufferId));
			delete it.layerPointer.spriteLayer;
		}
	}
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

void RenderLayer::addSpriteTriangle(int v1, int v2, int v3) {
	int layerId;
	if (!layers.empty() && layers.back().layerType == SPRITELAYER)
		layerId = layers.back().layerId;
	else {
		Layer newLayer;
		newLayer.layerType = SPRITELAYER;
		newLayer.layerId = spriteRenderer->addSpriteLayer();
		newLayer.layerPointer.spriteLayer = new SpriteLayer();
		layers.push_back(newLayer);
		layerId = layers.back().layerId;
	}
	spriteRenderer->flagTriangleForRender(layerId, v1, v2, v3);
	SpriteLayer* spriteLayer = layers.back().layerPointer.spriteLayer;
	spriteRenderer->addSpriteTriangleToLayer(spriteLayer, v1, v2, v3);
}

void RenderLayer::uploadLayerBuffers() {
	for (auto& it : layers) {
		if (it.layerType == SPRITELAYER) {
			spriteRenderer->uploadBuffers(it.layerId);
			it.layerPointer.spriteLayer->uploadBuffers();
		}
	}
}

void RenderLayer::clearLayers() {
	for (auto& it : layers) {
		if (it.layerType == SPRITELAYER) {
			spriteRenderer->removeSpriteLayer(it.layerId);
			glDeleteVertexArrays(1, &(it.layerPointer.spriteLayer->vaoId));
			glDeleteBuffers(1, &(it.layerPointer.spriteLayer->vertexBufferId));
			glDeleteBuffers(1, &(it.layerPointer.spriteLayer->indexBufferId));
			delete it.layerPointer.spriteLayer;
		}
	}
	layers.clear();
}

void RenderLayer::draw() {
	for (auto& it : layers) {
		if (it.layerType == SPRITELAYER) {
			spriteRenderer->drawLayer(it.layerId);
			spriteRenderer->drawSpriteLayer(it.layerPointer.spriteLayer);
		}
	}
}

}
