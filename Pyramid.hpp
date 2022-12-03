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
    glm::vec3 vertices[4];
    Pyramid(const char * vertex_path,const char * fragment_path);
    Pyramid();
    void initialize(GLuint prog, GLfloat x, GLfloat y, GLfloat z);
    void draw();
    void drawSecondary();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setRotation(GLfloat a, GLfloat b, GLfloat c);
    void findNearestFace(glm::vec3 point, glm::vec3 face[3]);

};


#endif
