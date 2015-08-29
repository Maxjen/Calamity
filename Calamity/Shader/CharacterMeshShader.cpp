#include "CharacterMeshShader.h"

namespace clm {

CharacterMeshShader::CharacterMeshShader() {
/*}

void SpriteShader::loadFromFiles(const char* vertexShader, const char* fragmentShader) {*/
	GLuint vertexShaderId = loadShader("Calamity/Shader/characterMesh.vert", GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader("Calamity/Shader/characterMesh.frag", GL_FRAGMENT_SHADER);
	/*GLuint vertexShaderId = loadShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fragmentShaderId = loadShader(fragmentShader, GL_FRAGMENT_SHADER);*/

	programId = glCreateProgram();
	glBindFragDataLocation(programId, 0, "out_Pos");
	glBindFragDataLocation(programId, 1, "out_Diffuse");
	glBindFragDataLocation(programId, 2, "out_Normal");
	glBindFragDataLocation(programId, 3, "out_TexCoord");
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
	boneIdsLocation = getAttribLocation("in_BoneIds");
	boneWeightsLocation = getAttribLocation("in_BoneWeights");
	//colorLocation = getAttribLocation("in_Color");

	projectionMatrixLocation = getUniformLocation("projectionMatrix");
	//viewMatrixLocation = getUniformLocation("viewMatrix");
	modelViewMatrixLocation = getUniformLocation("modelViewMatrix");
	modelMatrixLocation = getUniformLocation("modelMatrix");
	modelInverseTransposeLocation = getUniformLocation("modelInverseTranspose");
	//useTextureLocation = getUniformLocation("useTexture");

	for (unsigned int i = 0; i < 100; ++i) {
		char name[15];
		sprintf(name, "bones[%u]", i);
		bonesLocation[i] = getUniformLocation(name);
	}

	renderBlackLocation = getUniformLocation("renderBlack");

	GLint textureLocation = getUniformLocation("tex");

	bind();
	glUniform1i(textureLocation, 0);
	glUniform1i(renderBlackLocation, 0);
	unbind();
}

void CharacterMeshShader::setProjectionMatrix(glm::mat4 projectionMatrix) {
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

/*void CharacterMeshShader::setViewMatrix(glm::mat4 viewMatrix) {
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}*/

void CharacterMeshShader::setModelViewMatrix(glm::mat4 modelViewMatrix) {
	glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
}

void CharacterMeshShader::setModelMatrix(glm::mat4 modelMatrix) {
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void CharacterMeshShader::setModelInverseTranspose(glm::mat4 modelInverseTranspose) {
	glUniformMatrix4fv(modelInverseTransposeLocation, 1, GL_FALSE, glm::value_ptr(modelInverseTranspose));
}

/*void CharacterMeshShader::setUseTexture(int useTexture) {
	glUniform1i(useTextureLocation, useTexture);
}*/

void CharacterMeshShader::setBoneTransform(unsigned int index, const glm::mat4& boneTransform) {
	if (index < 100)
		glUniformMatrix4fv(bonesLocation[index], 1, GL_FALSE, glm::value_ptr(boneTransform));
}

void CharacterMeshShader::setRenderBlack(bool renderBlack) {
	if (renderBlack)
		glUniform1i(renderBlackLocation, 1);
	else
		glUniform1i(renderBlackLocation, 0);
}

GLuint CharacterMeshShader::createVertexArrayObject(GLuint vboId, GLsizei stride, const GLvoid* positionOffset, const GLvoid* normalOffset, const GLvoid* texCoordOffset,
													const GLvoid* boneIdsOffset, const GLvoid* boneWeightsOffset) {
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
	enableBoneIdsPointer();
	setBoneIdsPointer(stride, boneIdsOffset);
	enableBoneWeightsPointer();
	setBoneWeightsPointer(stride, boneWeightsOffset);
	/*enableColorPointer();
	setColorPointer(stride, colorOffset);*/

	glBindVertexArray(0);

	return vaoId;
}

void CharacterMeshShader::setPositionPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void CharacterMeshShader::setNormalPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void CharacterMeshShader::setTexCoordPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void CharacterMeshShader::setBoneIdsPointer(GLsizei stride, const GLvoid* offset) {
	//glVertexAttribPointer(boneIdsLocation, 4, GL_UNSIGNED_INT, GL_FALSE, stride, offset);
	glVertexAttribIPointer(boneIdsLocation, 4, GL_UNSIGNED_INT, stride, offset);
}

void CharacterMeshShader::setBoneWeightsPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(boneWeightsLocation, 4, GL_FLOAT, GL_FALSE, stride, offset);
}

/*void CharacterMeshShader::setColorPointer(GLsizei stride, const GLvoid* offset) {
	glVertexAttribPointer(colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, offset);
}*/

void CharacterMeshShader::enablePositionPointer() {
	glEnableVertexAttribArray(positionLocation);
}

void CharacterMeshShader::enableNormalPointer() {
	glEnableVertexAttribArray(normalLocation);
}

void CharacterMeshShader::enableTexCoordPointer() {
	glEnableVertexAttribArray(texCoordLocation);
}

void CharacterMeshShader::enableBoneIdsPointer() {
	glEnableVertexAttribArray(boneIdsLocation);
}

void CharacterMeshShader::enableBoneWeightsPointer() {
	glEnableVertexAttribArray(boneWeightsLocation);
}

/*void CharacterMeshShader::enableColorPointer() {
	glEnableVertexAttribArray(colorLocation);
}*/

void CharacterMeshShader::disablePositionPointer() {
	glDisableVertexAttribArray(positionLocation);
}

void CharacterMeshShader::disableNormalPointer() {
	glDisableVertexAttribArray(normalLocation);
}

void CharacterMeshShader::disableTexCoordPointer() {
	glDisableVertexAttribArray(texCoordLocation);
}

void CharacterMeshShader::disableBoneIdsPointer() {
	glDisableVertexAttribArray(boneIdsLocation);
}

void CharacterMeshShader::disableBoneWeightsPointer() {
	glDisableVertexAttribArray(boneWeightsLocation);
}

/*void CharacterMeshShader::disableColorPointer() {
	glDisableVertexAttribArray(colorLocation);
}*/


}
