#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

class SpriteShader : public Shader {
private:
	GLint positionLocation;
	GLint texCoordLocation;
	GLint colorLocation;
	GLint projectionMatrixLocation;
	GLint modelViewMatrixLocation;
	//GLint useTextureLocation;
	GLint renderBlackLocation;
	GLint anaglyphLocation;
public:
	explicit SpriteShader();
	//void loadFromFiles(const char* vertexShader, const char* fragmentShader);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);
	//void setUseTexture(int useTexture);
	void setRenderBlack(bool renderBlack);
	void setAnaglyph(bool bAnaglyph);

	GLuint createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* texCoordOffset, const GLvoid* colorOffset);

	void setPositionPointer(GLsizei stride, const GLvoid* offset);
	void setTexCoordPointer(GLsizei stride, const GLvoid* offset);
	void setColorPointer(GLsizei stride, const GLvoid* offset);

	void enablePositionPointer();
	void enableTexCoordPointer();
	void enableColorPointer();

	void disablePositionPointer();
	void disableTexCoordPointer();
	void disableColorPointer();
};

}
