#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

/*struct DirectionalLight {
	glm::vec3 color;
	float ambientIntensity;
};*/

class MeshShader : public Shader {
private:
	GLint positionLocation;
	GLint normalLocation;
	GLint texCoordLocation;
	//GLint colorLocation;
	GLint projectionMatrixLocation;
	GLint modelViewMatrixLocation;
	GLint modelMatrixLocation;
	GLint modelInverseTransposeLocation;
	//GLint useTextureLocation;
	GLint renderBlackLocation;
	GLint invertNormalsLocation;
	/*GLint dirLightColorLocation;
	GLint dirLightDirectionLocation;
	GLint dirLightAmbientIntesityLocation;
	GLint dirLightDiffuseIntesityLocation;*/
public:
	explicit MeshShader();
	//void loadFromFiles(const char* vertexShader, const char* fragmentShader);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);
	void setModelMatrix(glm::mat4 modelMatrix);
	void setModelInverseTranspose(glm::mat4 modelInverseTranspose);
	//void setUseTexture(int useTexture);
	void setRenderBlack(bool bRenderBlack);
	void setInvertNormals(bool bInvertNormals);
	//void setDirectionalLight(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity);

	GLuint createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* normalOffset, const GLvoid* texCoordOffset);
	GLuint createVertexArrayObject(GLuint vboPNId, GLuint vboTId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* normalOffset, const GLvoid* texCoordOffset);

	void setPositionPointer(GLsizei stride, const GLvoid* offset);
	void setNormalPointer(GLsizei stride, const GLvoid* offset);
	void setTexCoordPointer(GLsizei stride, const GLvoid* offset);
	//void setColorPointer(GLsizei stride, const GLvoid* offset);

	void enablePositionPointer();
	void enableNormalPointer();
	void enableTexCoordPointer();
	//void enableColorPointer();

	void disablePositionPointer();
	void disableNormalPointer();
	void disableTexCoordPointer();
	//void disableColorPointer();
};

}
