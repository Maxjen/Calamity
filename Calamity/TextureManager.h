#pragma once

#include <unordered_map>
#include <map>
#include <set>
#include <string>

#include "TextureAtlas.h"

using std::unordered_map;
using std::map;
using std::set;
using std::string;

namespace clm {

struct TextureBounds {
	float minU, minV, maxU, maxV;
};

struct AtlasData {
	TextureAtlas* atlas;
	set<unsigned int> textures;
	float wastedCapacity;
	bool isDirty;
};

class TextureManager {
private:
	struct Texture {
		unsigned int atlasId;
		TextureBounds bounds;
		int width, height;
		int counter;
	};

	//TextureAtlas* textureAtlas;
	//unordered_map<int, TextureAtlas*> textureAtlases;
	unordered_map<unsigned int, AtlasData> textureAtlases;
	unsigned int cTextureAtlas;

	size_t atlasDimension;
	//float wastedCapacity;

	map<string, unsigned int> textureIds;
	map<unsigned int, Texture> textures;
	unsigned int textureCount;
public:
	explicit TextureManager();
	~TextureManager();
	//void rebuild();

	//Texture* getTexture(const char* textureName);

	unsigned int createTexture(const char* textureName);
	void deleteTexture(unsigned int textureId);

	unsigned int getAtlasId(unsigned int textureId);
	TextureBounds getUvBounds(unsigned int textureId);
	int getTextureWidth(unsigned int textureId);
	int getTextureHeight(unsigned int textureId);

	//void writeToPng(const char* fileName);
	void writeToPng();

	void upload();

	void bind(GLenum textureUnit, unsigned int atlasId);
	void unbind();
};

}
