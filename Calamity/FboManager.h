#pragma once

#include <unordered_map>

#include "Shader/SpriteShader.h"
#include "Shader/RadialBlurShader.h"

namespace clm {

using std::unordered_map;

class FboManager {
private:
	struct SpriteVertexSimple {
		float x, y;
		float u, v;
		char r, g, b, a;
	};

	struct Fbo {
		GLuint fboId;
		GLuint fboTextureId;
	};

	unordered_map<unsigned int, Fbo> fbos;
	int cFbo;

	GLuint vertexBufferId;
	GLuint indexBufferId;
	GLuint vaoId;

	SpriteShader* spriteShader;
	RadialBlurShader* radialBlurShader;

	int screenWidth, screenHeight;
public:
	explicit FboManager(SpriteShader* spriteShader, RadialBlurShader* radialBlurShader, int screenWidth, int screenHeight);
	~FboManager();

	unsigned int addFbo();
	unsigned int addFboFloatingPoint();
	void removeFbo(unsigned int fboId);

	void bindFbo(unsigned int fboId);
	void unbindFbo();

	void drawFbo(unsigned int fboId);
};

}
