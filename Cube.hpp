#ifndef CUBE_HPP
#define CUBE_HPP

#include "DrawableObject.hpp"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Cube : public DrawableObject {
    private:
    void setBuffers();

    public:
    void initialize(GLuint prog);
    void draw();
    void drawSecondary();
};


#endif