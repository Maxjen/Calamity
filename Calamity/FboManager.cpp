#include "FboManager.h"

namespace clm {

FboManager::FboManager(SpriteShader* spriteShader, RadialBlurShader* radialBlurShader, int screenWidth, int screenHeight) :
	fbos(),
	cFbo(0),

	vertexBufferId(0),
	indexBufferId(0),
	vaoId(0),

	spriteShader(spriteShader),
	radialBlurShader(radialBlurShader),

	screenWidth(screenWidth),
	screenHeight(screenHeight)
{
	glGenBuffers(1, &vertexBufferId);
	glGenBuffers(1, &indexBufferId);
	//vaoId = spriteShader->createVertexArrayObject(vertexBufferId, sizeof(SpriteVertexSimple), 0, (GLvoid*)(2 * sizeof(float)), (GLvoid*)(4 * sizeof(float)));
	vaoId = radialBlurShader->createVertexArrayObject(vertexBufferId, sizeof(SpriteVertexSimple), 0, (GLvoid*)(2 * sizeof(float)), (GLvoid*)(4 * sizeof(float)));

	SpriteVertexSimple vertices[4];
	vertices[0].x = 0.0f;
	vertices[0].y = screenHeight;
	vertices[0].u = 0.0f;
	vertices[0].v = 1.0f;
	vertices[0].r = vertices[0].g = vertices[0].b = vertices[0].a = 255;
	//vertices[0].g = vertices[1].g = vertices[2].g = vertices[3].g = 0;

	vertices[1].x = 0.0f;
	vertices[1].y = 0.0f;
	vertices[1].u = 0.0f;
	vertices[1].v = 0.0f;
	vertices[1].r = vertices[1].g = vertices[1].b = vertices[1].a = 255;

	vertices[2].x = screenWidth;
	vertices[2].y = 0.0f;
	vertices[2].u = 1.0f;
	vertices[2].v = 0.0f;
	vertices[2].r = vertices[2].g = vertices[2].b = vertices[2].a = 255;

	vertices[3].x = screenWidth;
	vertices[3].y = screenHeight;
	vertices[3].u = 1.0f;
	vertices[3].v = 1.0f;
	vertices[3].r = vertices[3].g = vertices[3].b = vertices[3].a = 255;

	int indices[6] = {0, 1, 3, 3, 1, 2};

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(SpriteVertexSimple), &vertices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), &indices[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

FboManager::~FboManager() {
	for (auto& it : fbos) {
		glDeleteTextures(1, &(it.second.fboTextureId));
		glDeleteFramebuffers(1, &(it.second.fboId));
	}

	glDeleteBuffers(1, &vertexBufferId);
	glDeleteBuffers(1, &indexBufferId);
	glDeleteVertexArrays(1, &vaoId);
}

unsigned int FboManager::addFbo() {
	Fbo& fbo = fbos[cFbo];

	glGenFramebuffers(1, &(fbo.fboId));
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboId);

	glGenTextures(1, &(fbo.fboTextureId));
	glBindTexture(GL_TEXTURE_2D, fbo.fboTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.fboTextureId, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "INFO: framebuffer error. Exit\n");
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return cFbo++;
}

unsigned int FboManager::addFboFloatingPoint() {
	Fbo& fbo = fbos[cFbo];

	glGenFramebuffers(1, &(fbo.fboId));
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboId);

	glGenTextures(1, &(fbo.fboTextureId));
	glBindTexture(GL_TEXTURE_2D, fbo.fboTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.fboTextureId, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "INFO: framebuffer error. Exit\n");
		return -1;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return cFbo++;
}

void FboManager::removeFbo(unsigned int fboId) {
	Fbo& fbo = fbos[fboId];

	glDeleteTextures(1, &(fbo.fboTextureId));
	glDeleteFramebuffers(1, &(fbo.fboId));

	fbos.erase(fboId);
}

void FboManager::bindFbo(unsigned int fboId) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbos[fboId].fboId);
}

void FboManager::unbindFbo() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FboManager::drawFbo(unsigned int fboId) {
	Fbo& fbo = fbos[fboId];

	//spriteShader->bind();
	radialBlurShader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo.fboTextureId);
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	spriteShader->unbind();
}

}
