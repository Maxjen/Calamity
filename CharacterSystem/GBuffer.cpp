#include "GBuffer.h"

GBuffer::GBuffer() :
	fboId(0),
	textures(),
	depthTexture(0),
	finalTexture(0),
	screenWidth(0),
	screenHeight(0)
{
	for (unsigned int i = 0; i < 4; ++i)
		textures[i] = 0;
}

GBuffer::~GBuffer() {
	if (fboId)
		glDeleteFramebuffers(1, &fboId);

	for (unsigned int i = 0; i < 4; ++i) {
		if (textures[i])
			glDeleteTextures(1, &textures[i]);
	}

	if (depthTexture)
		glDeleteTextures(1, &depthTexture);

	if (finalTexture)
		glDeleteTextures(1, &finalTexture);
}

bool GBuffer::init(unsigned int screenWidth, unsigned int screenHeight) {
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);

	glGenTextures(4, textures);
	glGenTextures(1, &depthTexture);
	glGenTextures(1, &finalTexture);

	for (unsigned int i = 0; i < 4; ++i) {
		glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i], 0);
	}

	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth, screenHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	glBindTexture(GL_TEXTURE_2D, finalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGB, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, finalTexture, 0);

	/*GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, drawBuffers);*/

	//printf("maxDrawBuffers %u\n", GL_MAX_DRAW_BUFFERS);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		if (status == GL_FRAMEBUFFER_UNDEFINED) {
			printf("Framebuffer error: GL_FRAMEBUFFER_UNDEFINED\n");
		}
		else if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
			printf("Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
		}
		else if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
			printf("Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
		}
		else if (status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER) {
			printf("Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
		}
		else if (status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER) {
			printf("Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
		}
		else if (status == GL_FRAMEBUFFER_UNSUPPORTED) {
			printf("Framebuffer error: GL_FRAMEBUFFER_UNSUPPORTED\n");
		}
		else if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE) {
			printf("Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
		}
		else if (status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE) {
			printf("Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
		}
		else if (status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS) {
			printf("Framebuffer error: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");
		}
		else {
			printf("Framebuffer error: %u\n", (unsigned int)status);
		}
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void GBuffer::startFrame() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboId);
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawBuffer(GL_NONE);
}

void GBuffer::startGeometryPass() {
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, drawBuffers);

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glClearColor(0, 0, 0, 0);
	//glClearDepth(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::endGeometryPass() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);
	glDrawBuffer(GL_NONE);
}

void GBuffer::bindForStencilPass() {
	glDrawBuffer(GL_NONE);
}

void GBuffer::bindForLightPass() {
	glDrawBuffer(GL_COLOR_ATTACHMENT4);
	for (unsigned int i = 0; i < 4; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
}

void GBuffer::endFrame() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
	glReadBuffer(GL_COLOR_ATTACHMENT4);

	glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_READ_BUFFER, 0);
}

void GBuffer::setReadBuffer(unsigned int i) {
	if (i >= 0 && i < 4)
		glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
	else
		printf("Couldn't set read buffer to COLOR_ATTACHMENT%u\n", i);
}

void GBuffer::renderBuffers() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);

	GLsizei halfWidth = (GLsizei)(screenWidth / 2.0f);
	GLsizei halfHeight = (GLsizei)(screenHeight / 2.0f);

	setReadBuffer(0);
	glBlitFramebuffer(0, 0, screenWidth, screenHeight,
					0, halfHeight, halfWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	setReadBuffer(1);
	glBlitFramebuffer(0, 0, screenWidth, screenHeight,
					halfWidth, halfHeight, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	setReadBuffer(2);
	glBlitFramebuffer(0, 0, screenWidth, screenHeight,
					0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	setReadBuffer(3);
	glBlitFramebuffer(0, 0, screenWidth, screenHeight,
					halfWidth, 0, screenWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}
