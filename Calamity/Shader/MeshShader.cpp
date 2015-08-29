#include "MeshShader.h"

namespace clm {

MeshShader::MeshShader() {
/*}

void SpriteShader::loadFromFiles(const char* vertexShader, const char* fragmentShader) {*/
	GLuint vertexShaderId = loadShader("Calamity/Shader/mesh.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader("Calamity/Shader/mesh.frag", GL_FRAGMENT_SHADER);
	/*GLuint vertexShaderId = loadShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader(fragmentShader, GL_FRAGMENT_SHADER);*/

	programId = glCreateProgram();
	glBindFragDataLocation(programId, 0, "out_Pos");
	glBindFragDataLocation(programId, 1, "out_Diffuse");
	glBindFragDataLocation(programId, 2, "out_Normal");
	glBindFragDataLocation(programId, 3, "out_TexCoord");
	glBindFragDataLocation(programId, 4, "out_Outline");
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	// Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE) {
		printf("Error linking program %d!\n", programId);
		printProgramLog(programId);
		glDeleteProgram(programId);
		programId = 0;

		glDetachShader(programId, vertexShaderId);
		glDetachShader(programId, fragmentShaderId);

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return;
	}

	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	positionLocation = getAttribLocation("in_Position");
	normalLocation = getAttribLocation("in_Normal");
	texCoordLocation = getAttribLocation("in_TexCoord");
	//colorLocation = getAttribLocation("in_Color");

	projectionMatrixLocation = getUniformLocation("projectionMatrix");
	modelViewMatrixLocation = getUniformLocation("modelViewMatrix");
	modelMatrixLocation = getUniformLocation("modelMatrix");
	modelInverseTransposeLocation = getUniformLocation("modelInverseTranspose");
	//useTextureLocation = getUniformLocation("useTexture");
	renderBlackLocation = getUniformLocation("bRenderBlack");
	invertNormalsLocation = getUniformLocation("bInvertNormals");
	/*dirLightColorLocation = getUniformLocation("directionalLight.color");
	dirLightDirectionLocation = getUniformLocation("directionalLight.direction");
	dirLightAmbientIntesityLocation = getUniformLocation("directionalLight.ambientIntensity");
	dirLightDiffuseIntesityLocation = getUniformLocation("directionalLight.diffuseIntensity");*/

	GLint textureLocation = getUniformLocation("tex");

	bind();
	glUniform1i(textureLocation, 0);
	glUniform1i(renderBlackLocation, false);
	glUniform1i(invertNormalsLocation, false);
	unbind();
}

void MeshShader::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void MeshShader::setModelViewMatrix(glm::mat4 modelViewMatrix) {
	glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}

void MeshShader::setModelMatrix(glm::mat4 modelMatrix) {
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void MeshShader::setModelInverseTranspose(glm::mat4 modelInverseTranspose) {
	glUniformMatrix4fv(modelInverseTransposeLocation, 1, GL_FALSE, glm::value_ptr(modelInverseTranspose));
}

/*void MeshShader::setUseTexture(int useTexture) {
	glUniform1i(useTextureLocation, useTexture);
}*/

void MeshShader::setRenderBlack(bool bRenderBlack) {
	glUniform1i(renderBlackLocation, bRenderBlack);
}

void MeshShader::setInvertNormals(bool bInvertNormals) {
	glUniform1i(invertNormalsLocation, bInvertNormals);
}

/*void MeshShader::setDirectionalLight(glm::vec3 color, glm::vec3 direction, float ambientIntensity, float diffuseIntensity) {
	glUniform3f(dirLightColorLocation, color.x, color.y, color.z);
	glm::vec3 directionN = glm::normalize(direction);
	glUniform3f(dirLightDirectionLocation, directionN.x, directionN.y, directionN.z);
	glUniform1f(dirLightAmbientIntesityLocation, ambientIntensity);
	glUniform1f(dirLightDiffuseIntesityLocation, diffuseIntensity);
}*/

GLuint MeshShader::createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* normalOffset, const GLvoid* texCoordOffset) {
	GLuint vaoId;
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	enablePositionPointer();
	setPositionPointer(stride, positionOffset);
	enableNormalPointer();
	setNormalPointer(stride, normalOffset);
	enableTexCoordPointer();
	setTexCoordPointer(stride, texCoordOffset);
	/*enableColorPointer();
	setColorPointer(stride, colorOffset);*/

	glBindVertexArray(0);

	return vaoId;
}

GLuint MeshShader::createVertexArrayObject(GLuint vboPNId, GLuint vboTId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* normalOffset, const GLvoid* texCoordOffset) {
	GLuint vaoId;
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, vboPNId);
	enablePositionPointer();
	setPositionPointer(stride, positionOffset);
	enableNormalPointer();
	setNormalPointer(stride, normalOffset);
	glBindBuffer(GL_ARRAY_BUFFER, vboTId);
	enableTexCoordPointer();
	setTexCoordPointer(0, texCoordOffset);
	/*enableColorPointer();
	setColorPointer(stride, colorOffset);*/

	glBindVertexArray(0);

	return vaoId;
}

void MeshShader::setPositionPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void MeshShader::setNormalPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void MeshShader::setTexCoordPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

/*void MeshShader::setColorPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, offset);
}*/

void MeshShader::enablePositionPointer() {
	glEnableVertexAttribArray(positionLocation);
}

void MeshShader::enableNormalPointer() {
	glEnableVertexAttribArray(normalLocation);
}

void MeshShader::enableTexCoordPointer() {
	glEnableVertexAttribArray(texCoordLocation);
}

/*void MeshShader::enableColorPointer() {
	glEnableVertexAttribArray(colorLocation);
}*/

void MeshShader::disablePositionPointer() {
	glDisableVertexAttribArray(positionLocation);
}

void MeshShader::disableNormalPointer() {
	glDisableVertexAttribArray(normalLocation);
}

void MeshShader::disableTexCoordPointer() {
	glDisableVertexAttribArray(texCoordLocation);
}

/*void MeshShader::disableColorPointer() {
	glDisableVertexAttribArray(colorLocation);
}*/


}
