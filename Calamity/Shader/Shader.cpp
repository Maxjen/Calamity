#include "Shader.h"

namespace clm {

Shader::Shader() :
	programId(0)
{}

Shader::~Shader() {
	glDeleteProgram(programId);
}

void Shader::bind() {
	glUseProgram(programId);

	//Check for error
	/*GLenum error = glGetError();
	if( error != GL_NO_ERROR )
	{
		printf( "Error binding shader! %s\n", gluErrorString( error ) );
		printProgramLog( mProgramID );
		return false;
	}

	return true;*/
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::printProgramLog(GLuint program) {
	// Make sure name is shader
	if (glIsProgram(program)) {
		// Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		// Get info string length
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// Allocate string
		char* infoLog = new char[maxLength];

		// Get info log
		glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
		if (infoLogLength > 0){
			// Print Log
			printf("%s\n", infoLog);
		}

		// Deallocate string
		delete[] infoLog;
	}
	else {
		printf("Name %d is not a program\n", program);
	}
}

void Shader::printShaderLog(GLuint shader) {
	//Make sure name is shader
	if (glIsShader(shader)) {
		// Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;

		// Get info string length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// Allocate string
		char* infoLog = new char[maxLength];

		//Get info log
		glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
		if(infoLogLength > 0) {
			// Print Log
			printf("%s\n", infoLog);
		}

		//Deallocate string
		delete[] infoLog;
	}
	else {
		printf("Name %d is not a shader\n", shader);
	}
}

GLuint Shader::loadShader(const char* fileName, GLenum shaderType) {
	GLuint shaderId = glCreateShader(shaderType);

	string shaderString = fileToString(fileName);
	const GLchar* shaderSource = shaderString.c_str();
	glShaderSource(shaderId, 1, (const GLchar**)&shaderSource, NULL);

	glCompileShader(shaderId);

	// Check shader for errors
	GLint shaderCompiled = GL_FALSE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE)
	{
		printf("Unable to compile shader %d!\n\nSource:\n%s\n", shaderId, shaderSource);
		printShaderLog(shaderId);
		glDeleteShader(shaderId);
		shaderId = 0;
	}

	return shaderId;
}

string Shader::fileToString(const char* fileName) {
	FILE* fp = fopen(fileName, "rb");
	if (fp) {
		string contents;
		fseek(fp, 0, SEEK_END);
		contents.resize(ftell(fp));
		rewind(fp);
		fread(&contents[0], 1, contents.size(), fp);
		fclose(fp);
		return(contents);
	}
	printf("Unable to open shader file \"%s\"\n", fileName);
	return "";
}

GLint Shader::getAttribLocation(const char* attribName) {
	GLint location = glGetAttribLocation(programId, attribName);
	if (location == -1)
		printf("%s is not a valid glsl program variable!\n", attribName);
	return location;
}

GLint Shader::getUniformLocation(const char* uniformName) {
	GLint location = glGetUniformLocation(programId, uniformName);
	if (location == -1)
		printf( "%s is not a valid glsl program variable!\n", uniformName);
	return location;
}

}
