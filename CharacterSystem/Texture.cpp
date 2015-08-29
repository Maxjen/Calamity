#include "Texture.h"
#include "SDL2/SDL_image.h"

Texture::Texture() :
	textureId(0),
	bLoaded(false)
{}

bool Texture::load(const char* fileName) {
	SDL_Surface* textureImage;
	if (!(textureImage = IMG_Load(fileName))) {
		fprintf(stderr, "INFO: couldn't load image from file. (%s) Exit\n", fileName);
		fprintf(stderr, "%s\n", IMG_GetError());
		return false;
	}

	if (textureImage->format->BytesPerPixel == 3) {
		printf("%s\n", fileName);
		printf("%x\n", textureImage->format->Rmask);
		textureImage = SDL_ConvertSurfaceFormat(textureImage, SDL_PIXELFORMAT_ABGR8888, 0);
		//SDL_SetSurfaceAlphaMod(textureImage, 255);
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int depth = textureImage->format->BytesPerPixel;
	int width = textureImage->w;
	int height = textureImage->h;
	if (depth == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureImage->pixels);
	else if (depth == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->pixels);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, textureImage->pixels);

	SDL_FreeSurface(textureImage);
	bLoaded = true;
	return true;
}

Texture::~Texture() {
	if (bLoaded)
		glDeleteTextures(1, &textureId);
}

void Texture::bind(GLenum textureUnit) {
	if (bLoaded) {
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
