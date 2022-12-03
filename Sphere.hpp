#ifndef SPHERE_HPP
#define SPHERE_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "DrawableObject.hpp"




#define STACKS 30
#define SECTORS 30

extern GLuint N;

class Sphere : public DrawableObject {
    private:
    GLfloat rot[3];
    glm::vec3 pos;
    GLfloat radius;
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    std::vector<GLuint> indices;
    std::vector<GLuint> lineIndices;
    GLuint ebo;

    void setBuffers();

    public:
    Sphere(const char * vertex_path,const char * fragment_path);
    Sphere();
    void initialize(GLuint prog);
    void draw();
    void drawSecondary();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setRotation(GLfloat a, GLfloat b, GLfloat c);
    void moveForward(GLfloat speed, GLfloat delta);
    void moveBackward(GLfloat speed, GLfloat delta);
    void moveLeft(GLfloat speed, GLfloat delta);
    void moveRight(GLfloat speed, GLfloat delta);
    glm::vec3 getCenter();
    GLfloat getRadius();

};


#endif
