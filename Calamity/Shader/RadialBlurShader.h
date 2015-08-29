#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

class RadialBlurShader : public Shader {
private:
	int screenWidth, screenHeight;

	GLint positionLocation;
	GLint texCoordLocation;
	GLint colorLocation;
	GLint projectionMatrixLocation;
	GLint modelViewMatrixLocation;
	//GLint useTextureLocation;
public:
	explicit RadialBlurShader(int screenWidth, int screenHeight);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);
	void setBlurOrigin(float x, float y);
	//void setUseTexture(int useTexture);

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
