#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

class TextureShader : public Shader {
private:
	GLint positionLocation;
	GLint texCoordLocation;
	GLint colorLocation;
	GLint texOriginLocation;
	GLint texDimensionsLocation;
	GLint projectionMatrixLocation;
	GLint modelViewMatrixLocation;
	//GLint useTextureLocation;
	GLint renderBlackLocation;
	GLint anaglyphLocation;
public:
	explicit TextureShader();
	//void loadFromFiles(const char* vertexShader, const char* fragmentShader);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);
	//void setUseTexture(int useTexture);
	void setRenderBlack(bool renderBlack);
	void setAnaglyph(bool bAnaglyph);

	GLuint createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* texCoordOffset, const GLvoid* colorOffset, const GLvoid* texOriginOffset, const GLvoid* texDimensionsOffset);

	void setPositionPointer(GLsizei stride, const GLvoid* offset);
	void setTexCoordPointer(GLsizei stride, const GLvoid* offset);
	void setColorPointer(GLsizei stride, const GLvoid* offset);
	void setTexOriginPointer(GLsizei stride, const GLvoid* offset);
	void setTexDimensionsPointer(GLsizei stride, const GLvoid* offset);

	void enablePositionPointer();
	void enableTexCoordPointer();
	void enableColorPointer();
	void enableTexOriginPointer();
	void enableTexDimensionsPointer();

	void disablePositionPointer();
	void disableTexCoordPointer();
	void disableColorPointer();
	void disableTexOriginPointer();
	void disableTexDimensionsPointer();
};

}
