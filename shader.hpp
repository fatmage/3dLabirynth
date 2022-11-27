#ifndef SHADER_HPP
#define SHADER_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

class Program {
    private:
    GLuint programID;
    public:
    ~Program();
    Program();
    Program(const char * vertex_path,const char * fragment_path);
    GLuint LoadShaders(const char * vertex_file_path,const char * fragment_path);
    void use();
    void setProgram(const char * vertex_path,const char * fragment_path);
    GLuint getProgID() {
        return programID;
    }
    void deleteProg() {
        if (programID);
            glDeleteProgram(programID);
    }
};


#endif
