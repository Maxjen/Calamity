#pragma once

#include <cstdio>
#include <GL/glew.h>

//using namespace clm;

class GBuffer {
private:
	GLuint fboId;
	GLuint textures[4];
	GLuint depthTexture;
	GLuint finalTexture;

	unsigned int screenWidth, screenHeight;
public:
	explicit GBuffer();
	~GBuffer();

	bool init(unsigned int screenWidth, unsigned int screenHeight);

	void startFrame();
	void startGeometryPass();
	void endGeometryPass();
	void bindForStencilPass();
	void bindForLightPass();
	void endFrame();

	void setReadBuffer(unsigned int i);
	void renderBuffers();
};
