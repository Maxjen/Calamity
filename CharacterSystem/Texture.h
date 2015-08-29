#pragma once

#include <GL/glew.h>

#include <string>

using std::string;

class Texture {
private:
	GLuint textureId;
	bool bLoaded;
public:
	explicit Texture();
	~Texture();

	bool load(const char* fileName);
	void bind(GLenum textureUnit);
	void unbind();
};
