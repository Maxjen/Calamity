#pragma once

#include <GL/glew.h>
#include <string>

using std::string;

namespace clm {

class Shader {
protected:
	GLuint programId;
	void printProgramLog(GLuint program);
	void printShaderLog(GLuint shader);
	GLuint loadShader(const char* fileName, GLenum shaderType);
	string fileToString(const char* fileName);
	GLint getAttribLocation(const char* attribName);
	GLint getUniformLocation(const char* uniformName);
public:
	explicit Shader();
	virtual ~Shader();

	void bind();
	void unbind();
};

}
