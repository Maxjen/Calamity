#include "TextureManager.h"
#include "SDL2/SDL_image.h"

namespace clm {

TextureManager::TextureManager() :
	textureAtlases(),
	cTextureAtlas(0),

	atlasDimension(2048),

	textureIds(),
	textures(),
	textureCount(0)
{
	textureAtlases[cTextureAtlas].atlas = new TextureAtlas(atlasDimension, atlasDimension, 4);
	textureAtlases[cTextureAtlas].wastedCapacity = 0;
	textureAtlases[cTextureAtlas].isDirty = false;
}

TextureManager::~TextureManager() {
	for (auto& it : textureAtlases)
		delete it.second.atlas;

	//delete textureAtlas;
}

/*void TextureManager::rebuild() {
	delete textureAtlas;
	wastedCapacity = 0;
	textureAtlas = new TextureAtlas(atlasDimension, atlasDimension, 4);

	map<string, int>::iterator it;
	for (it = textureIds.begin(); it != textureIds.end(); ++it) {
		int id = it->second;

		SDL_Surface* textureImage;
		if (!(textureImage = IMG_Load((it->first).c_str()))) {
			fprintf(stderr, "INFO: couldn't load image from file. Atlas rebuild failed. Exit\n");
			return;
		}

		AtlasRegion region = textureAtlas->getRegion(textureImage->w, textureImage->h);
		if (region.x == -1) {
			SDL_FreeSurface(textureImage);
			atlasDimension *= 2;
			rebuild();
			return;
		}

		textureAtlas->setRegion(region.x, region.y, region.width, region.height, (unsigned char*)textureImage->pixels, textureImage->w * 4);

		textures[id].bounds.minU = ((float)region.x) / atlasDimension;
		textures[id].bounds.maxU = ((float)region.x + region.width) / atlasDimension;
		textures[id].bounds.minV = ((float)region.y) / atlasDimension;
		textures[id].bounds.maxV = ((float)region.y + region.height) / atlasDimension;

		SDL_FreeSurface(textureImage);
	}
}*/

unsigned int TextureManager::createTexture(const char* textureName) {
	map<string, unsigned int>::iterator it = textureIds.find(textureName);

	if (it != textureIds.end()) {
		textures[it->second].counter++;
		return it->second;
	}
	else {
		SDL_Surface* textureImage;
		if (!(textureImage = IMG_Load(textureName))) {
			fprintf(stderr, "INFO: couldn't load image from file. (%s) Exit\n", textureName);
			fprintf(stderr, "%s\n", IMG_GetError());
			return -1;
		}

		if (textureImage->format->BytesPerPixel == 3) {
			printf("%s\n", textureName);
			printf("%x\n", textureImage->format->Rmask);
			textureImage = SDL_ConvertSurfaceFormat(textureImage, SDL_PIXELFORMAT_ABGR8888, 0);
			//SDL_SetSurfaceAlphaMod(textureImage, 255);
		}

		if (textureImage->w > 2000 || textureImage->h > 2000) {
			fprintf(stderr, "INFO: couldn't load image from file. Image resolution too big. Exit\n");
			return -1;
		}

		AtlasRegion region = textureAtlases[cTextureAtlas].atlas->getRegion(textureImage->w, textureImage->h);

		/*while (region.x == -1) {
			atlasDimension *= 2;
			rebuild();
			region = textureAtlas->getRegion(textureImage->w, textureImage->h);
		}*/
		if (region.x == -1) {
			cTextureAtlas++;
			textureAtlases[cTextureAtlas].atlas = new TextureAtlas(atlasDimension, atlasDimension, 4);
			textureAtlases[cTextureAtlas].wastedCapacity = 0;
			region = textureAtlases[cTextureAtlas].atlas->getRegion(textureImage->w, textureImage->h);
		}
		textureIds[textureName] = textureCount;
		int id = textureCount++;

		textureAtlases[cTextureAtlas].atlas->setRegion(region.x, region.y, region.width, region.height, (unsigned char*)textureImage->pixels, textureImage->w * 4);
		textureAtlases[cTextureAtlas].textures.insert(id);
		textureAtlases[cTextureAtlas].isDirty = true;

		textures[id].counter = 1;

		textures[id].atlasId = cTextureAtlas;
		textures[id].bounds.minU = ((float)region.x) / atlasDimension;
		textures[id].bounds.maxU = ((float)region.x + region.width) / atlasDimension;
		textures[id].bounds.minV = ((float)region.y) / atlasDimension;
		textures[id].bounds.maxV = ((float)region.y + region.height) / atlasDimension;
		textures[id].width = region.width;
		textures[id].height = region.height;

		SDL_FreeSurface(textureImage);
		return id;
		/*AtlasRegion region = textureAtlas->getRegion(textureImage->w, textureImage->h);
		while (region.x == -1) {
			atlasDimension *= 2;
			rebuild();
			region = textureAtlas->getRegion(textureImage->w, textureImage->h);
		}
		textureAtlas->setRegion(region.x, region.y, region.width, region.height, (unsigned char*)textureImage->pixels, textureImage->w * 4);

		textureIds[textureName] = textureCount;
		int id = textureCount++;

		textures[id].counter = 1;

		textures[id].bounds.minU = ((float)region.x) / atlasDimension;
		textures[id].bounds.maxU = ((float)region.x + region.width) / atlasDimension;
		textures[id].bounds.minV = ((float)region.y) / atlasDimension;
		textures[id].bounds.maxV = ((float)region.y + region.height) / atlasDimension;
		textures[id].width = region.width;
		textures[id].height = region.height;

		SDL_FreeSurface(textureImage);
		return id;*/
	}
}

void TextureManager::deleteTexture(unsigned int textureId) {
	map<unsigned int, Texture>::iterator it = textures.find(textureId);

	if (it == textures.end()) {
		printf("delete: texture with id %d doesn't exist\n", textureId);
		return;
	}
	else {
		textures[textureId].counter--;
		if (textures[textureId].counter == 0) {
			int atlasId = textures[textureId].atlasId;
			textureAtlases[atlasId].wastedCapacity += (textures[textureId].bounds.maxU - textures[textureId].bounds.minU) * (textures[textureId].bounds.maxV - textures[textureId].bounds.minV);
			//wastedCapacity += (textures[textureId].bounds.maxU - textures[textureId].bounds.minU) * (textures[textureId].bounds.maxV - textures[textureId].bounds.minV);
			textureAtlases[atlasId].textures.erase(textureId);
			textures.erase(textureId);

			map<string, unsigned int>::iterator itId;
			for (itId = textureIds.begin(); itId != textureIds.end(); ++itId) {
				if (itId->second == textureId) {
					textureIds.erase(itId);
					break;
				}
			}

			if (textureAtlases[atlasId].wastedCapacity >= 0.5f) {
				for (auto& it : textureAtlases[atlasId].textures) {
					for (itId = textureIds.begin(); itId != textureIds.end(); ++itId) {
						if (itId->second == it) {
							SDL_Surface* textureImage;
							if (!(textureImage = IMG_Load(itId->first.c_str()))) {
								fprintf(stderr, "INFO: couldn't load image from file. Exit\n");
								//return -1;
							}

							/*if (textureImage->w > 2000 || textureImage->h > 2000) {
								fprintf(stderr, "INFO: couldn't load image from file. Image resolution too big. Exit\n");
								return -1;
							}*/

							AtlasRegion region = textureAtlases[cTextureAtlas].atlas->getRegion(textureImage->w, textureImage->h);

							if (region.x == -1) {
								cTextureAtlas++;
								textureAtlases[cTextureAtlas].atlas = new TextureAtlas(atlasDimension, atlasDimension, 4);
								textureAtlases[cTextureAtlas].wastedCapacity = 0;
								region = textureAtlases[cTextureAtlas].atlas->getRegion(textureImage->w, textureImage->h);
							}

							textureAtlases[cTextureAtlas].atlas->setRegion(region.x, region.y, region.width, region.height, (unsigned char*)textureImage->pixels, textureImage->w * 4);
							textureAtlases[cTextureAtlas].textures.insert(it);

							textures[it].atlasId = cTextureAtlas;
							textures[it].bounds.minU = ((float)region.x) / atlasDimension;
							textures[it].bounds.maxU = ((float)region.x + region.width) / atlasDimension;
							textures[it].bounds.minV = ((float)region.y) / atlasDimension;
							textures[it].bounds.maxV = ((float)region.y + region.height) / atlasDimension;
							textures[it].width = region.width;
							textures[it].height = region.height;

							SDL_FreeSurface(textureImage);
							break;
						}
					}
				}
				delete textureAtlases[atlasId].atlas;
				textureAtlases.erase(atlasId);
			}
			/*if (wastedCapacity >= 0.5f)
				rebuild();*/
		}
	}
}

unsigned int TextureManager::getAtlasId(unsigned int textureId) {
	return textures[textureId].atlasId;
}

TextureBounds TextureManager::getUvBounds(unsigned int textureId) {
	map<unsigned int, Texture>::iterator it = textures.find(textureId);

	if (it == textures.end()) {
		printf("getUvBounds: texture with id %d doesn't exist\n", textureId);
		TextureBounds bounds;
		bounds.minU = bounds.minV = bounds.maxU = bounds.maxV = -1;
		return bounds;
	}
	else {
		return textures[textureId].bounds;
	}
}

int TextureManager::getTextureWidth(unsigned int textureId) {
	return textures[textureId].width;
}

int TextureManager::getTextureHeight(unsigned int textureId) {
	return textures[textureId].height;
}

void TextureManager::writeToPng() {
	string fileName = "Test0.png";
	int i;
	for (auto& it : textureAtlases) {
		fileName[4] = '0' + i;
		it.second.atlas->writeToPng(fileName.c_str());
		i++;
	}

	//textureAtlas->writeToPng(fileName);
}

void TextureManager::upload() {
	// TODO: upload only where needed
	for (auto& it : textureAtlases) {
		if (it.second.isDirty) {
			it.second.atlas->upload();
			it.second.isDirty = false;
		}
	}

	//textureAtlas->upload();
}

void TextureManager::bind(GLenum textureUnit, unsigned int atlasId) {
	textureAtlases[atlasId].atlas->bind(textureUnit);
	//textureAtlas->bind(textureUnit);
}

void TextureManager::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

}
