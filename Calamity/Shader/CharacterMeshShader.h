#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

namespace clm {

class CharacterMeshShader : public Shader {
private:
	GLint positionLocation;
	GLint normalLocation;
	GLint texCoordLocation;
	GLint boneIdsLocation;
	GLint boneWeightsLocation;
	//GLint colorLocation;

	GLint projectionMatrixLocation;
	//GLint viewMatrixLocation;
	GLint modelViewMatrixLocation;
	GLint modelMatrixLocation;
	GLint modelInverseTransposeLocation;

	//GLint useTextureLocation;
	GLint bonesLocation[100];
	GLint renderBlackLocation;
public:
	explicit CharacterMeshShader();
	//void loadFromFiles(const char* vertexShader, const char* fragmentShader);

	void setProjectionMatrix(glm::mat4 projectionMatrix);
	//void setViewMatrix(glm::mat4 viewMatrix);
	void setModelViewMatrix(glm::mat4 modelViewMatrix);
	void setModelMatrix(glm::mat4 modelMatrix);
	void setModelInverseTranspose(glm::mat4 modelInverseTranspose);
	//void setUseTexture(int useTexture);
	void setBoneTransform(unsigned int index, const glm::mat4& boneTransform);
	void setRenderBlack(bool renderBlack);

	GLuint createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* normalOffset, const GLvoid* texCoordOffset,
								   const GLvoid* boneIdsOffset, const GLvoid* boneWeightsOffset);

	void setPositionPointer(GLsizei stride, const GLvoid* offset);
	void setNormalPointer(GLsizei stride, const GLvoid* offset);
	void setTexCoordPointer(GLsizei stride, const GLvoid* offset);
	void setBoneIdsPointer(GLsizei stride, const GLvoid* offset);
	void setBoneWeightsPointer(GLsizei stride, const GLvoid* offset);
	//void setColorPointer(GLsizei stride, const GLvoid* offset);

	void enablePositionPointer();
	void enableNormalPointer();
	void enableTexCoordPointer();
	void enableBoneIdsPointer();
	void enableBoneWeightsPointer();
	//void enableColorPointer();

	void disablePositionPointer();
	void disableNormalPointer();
	void disableTexCoordPointer();
	void disableBoneIdsPointer();
	void disableBoneWeightsPointer();
	//void disableColorPointer();
};

}
