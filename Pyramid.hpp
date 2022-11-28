#ifndef PYRAMID_HPP
#define PYRAMID_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "DrawableObject.hpp"

extern GLuint N;

class Pyramid : public DrawableObject {
    private:
    GLfloat rot[3];
    GLfloat pos[3];
    void setBuffers();



    public:
    Pyramid(const char * vertex_path,const char * fragment_path);
    Pyramid();
    void initialize(GLuint prog);
    void draw();
    void draw_lines();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setRotation(GLfloat a, GLfloat b, GLfloat c);

};


#endif
