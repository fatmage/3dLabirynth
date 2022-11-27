#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.hpp"

extern GLFWwindow* window;

void Program::use() {
	glUseProgram(this->programID);
}

Program::Program(const char *vertex_path, const char *fragment_path) {
	this->programID = LoadShaders(vertex_path, fragment_path);
}

Program::Program() {
}

Program::~Program() {
		glDeleteProgram(this->programID);
}
void Program::setProgram(const char * vertex_path,const char * fragment_path) {
	programID = LoadShaders(vertex_path, fragment_path);
}
GLuint Program::LoadShaders(const char * vertex_path,const char * fragment_path){

	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vsCode;
	std::ifstream VertexShaderStream(vertex_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		vsCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string fsCode;
	std::ifstream FragmentShaderStream(fragment_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		fsCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_path);
	char const *vsPointer = vsCode.c_str();
	glShaderSource(vsID, 1, &vsPointer , NULL);
	glCompileShader(vsID);

	// Check Vertex Shader
	glGetShaderiv(vsID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(vsID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_path);
	char const * FragmentSourcePointer = fsCode.c_str();
	glShaderSource(fsID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(fsID);

	// Check Fragment Shader
	glGetShaderiv(fsID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(fsID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}




	GLuint programID = glCreateProgram();
	glAttachShader(programID, vsID);
	glAttachShader(programID, fsID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(programID, vsID);
	glDetachShader(programID, fsID);
	
	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return programID;
}


