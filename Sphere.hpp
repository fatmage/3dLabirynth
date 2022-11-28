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
#include "shader.hpp"
#include "DrawableObject.hpp"




#define STACKS 30
#define SECTORS 30

extern GLuint N;

class Sphere : public DrawableObject {
    private:
    GLfloat rot[3];
    GLfloat pos[3];
    GLfloat radius;
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;
    std::vector<GLuint> indices;
    std::vector<GLuint> lineIndices;
    GLuint ebo;

    void setBuffers();
    void createBuffers();



    public:
    Sphere(const char * vertex_path,const char * fragment_path);
    Sphere();
    void initialize(GLuint prog);
    void draw();
    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setRotation(GLfloat a, GLfloat b, GLfloat c);

};


#endif
