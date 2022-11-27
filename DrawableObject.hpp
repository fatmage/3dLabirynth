#ifndef DRAWABLEOBJECT_HPP
#define DRAWABLEOBJECT_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <stdio.h>

#include "shader.hpp"

class DrawableObject {
    protected:
    GLuint vao, vbo;
    Program prog;
    void createBuffers() {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
    }

    public:
    DrawableObject() {
      createBuffers();
    }
    ~DrawableObject() {    // Cleanup VBO,VBO,Prog
      glDeleteBuffers(1, &vbo);
      glDeleteVertexArrays(1, &vao);
      prog.deleteProg();
    }
    void bindVAO() {
        glBindVertexArray(vao);
    }
    void bindProgram() {
        prog.use();
    }
   void bindBuffers() {
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }
    void loadProgram(const char * vertex_path,const char * fragment_path) {
        prog.setProgram(vertex_path, fragment_path);
    }
    void setProgram(Program pg) {
        prog = pg;
    }
    void setUniform3f(const char* name, GLfloat v1, GLfloat v2, GLfloat v3) {
        int location = glGetUniformLocation(prog.getProgID(), name);
        if (location >= 0) {
            bindProgram();
            glUniform3f(location, v1, v2, v3);
        } else {
            printf("No uniform variable in shader of program %d named %s!\n",prog.getProgID(), name);
        }
    }
    void setUniform1f(const char* name, GLfloat v) {
        int location = glGetUniformLocation(prog.getProgID(), name);
        if (location >= 0) {
            bindProgram();
            glUniform1f(location, v);
        } else {
            printf("No uniform variable in shader of program %d named %s!\n",prog.getProgID(), name);
        }
    }
    void setUniformmat4(const char* name, glm::mat4 v) {
        int location = glGetUniformLocation(prog.getProgID(), name);
        if (location >= 0) {
            bindProgram();
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
        } else {
            printf("No uniform variable in shader of program %d named %s!\n",prog.getProgID(), name);
        }
    }

};


#endif
