#include "Cube.hpp"
#include "Camera.hpp"

extern GLuint N;

extern Camera camera;

void Cube::initialize(GLuint prog) {
    programID = prog;
    bindBuffers();
    setBuffers();
}

void Cube::setBuffers() {
    const glm::vec3 vertices[8] = {
        glm::vec3(-0.5,     -0.5,                 -0.5),           // tylny face    0
        glm::vec3((GLfloat)N-0.5, -0.5,           -0.5),           //               1
        glm::vec3((GLfloat)N-0.5, (GLfloat)N-0.5, -0.5),           //               2
        glm::vec3(-0.5,           (GLfloat)N-0.5, -0.5),           //               3

        glm::vec3(-0.5,     -0.5,                 (GLfloat)N-0.5), // przedni face  4
        glm::vec3((GLfloat)N-0.5, -0.5,           (GLfloat)N-0.5), //               5
        glm::vec3((GLfloat)N-0.5, (GLfloat)N-0.5, (GLfloat)N-0.5), //               6
        glm::vec3(-0.5,           (GLfloat)N-0.5, (GLfloat)N-0.5)  //               7
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3,
        1, 5, 2,
        2, 5, 6,
        4, 5, 6,
        4, 6, 7,
        0, 4, 7,
        0, 7, 3,
        0, 1, 4,
        4, 1, 5,
        2, 3, 7,
        2, 7, 6
    };



    glBufferData(GL_ARRAY_BUFFER, 8*sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36*sizeof(GLuint), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}


void Cube::draw() {
	bindProgram();
    bindVAO();
    
    glm::mat4 viewmat = camera.getMainView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getMainProjection(); 
    setUniformmat4("projection", projectionmat);

    setUniform1f("N", N);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);  
}

void Cube::drawSecondary() {
	bindProgram();
    bindVAO();
    
    glm::mat4 viewmat = camera.getSecondaryView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getSecondaryProjection(); 
    setUniformmat4("projection", projectionmat);

    setUniform1f("N", N);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);  
}
