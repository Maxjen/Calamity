#pragma once

#include <vector>

#include "Renderer.h"

using std::vector;

namespace clm {

class RenderLayer {
private:
	enum LayerType {
		SPRITE_LAYER,
		TEXTURE_TRIANGLE_LAYER,
		FONT_LAYER,
		LINE_LAYER,
		TRIANGLE_LAYER,
	};

	union LayerPointer {
		SpriteLayer* spriteLayer;
		TextureTriangleLayer* textureTriangleLayer;
		FontLayer* fontLayer;
		LineLayer* lineLayer;
		TriangleLayer* triangleLayer;
	};

	struct Layer {
		LayerType layerType;
		//int layerId;
		LayerPointer layerPointer;
	};

	Renderer* renderer;

	vector<Layer> layers;
public:
	explicit RenderLayer(Renderer* renderer);
	~RenderLayer();

	Renderer* getRenderer();

	//int getSpriteLayer();
	void addSpriteTriangle(unsigned int v1, unsigned int v2, unsigned int v3);
	void addTextureTriangle(unsigned int v1, unsigned int v2, unsigned int v3);
	void addLetter(wchar_t letter, unsigned int fontId, float x, float y, char r, char g, char b, char a);
	void addLine(unsigned int v1, unsigned int v2);
	void addTriangle(unsigned int v1, unsigned int v2, unsigned int v3);

	void uploadLayerBuffers();
	void clearLayers();

	void draw();
};

}
