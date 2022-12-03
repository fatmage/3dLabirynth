#include "Pyramid.hpp"
#include "Camera.hpp"
#include <iostream>

extern GLuint LoadShaders(const char * vertex_path,const char * fragment_path);

extern GLuint N;

extern Camera camera;


Pyramid::Pyramid(const char * vertex_path,const char * fragment_path) {
    createBuffers();
    programID = LoadShaders(vertex_path, fragment_path);
    setBuffers();
}

Pyramid::Pyramid() {
}


void Pyramid::initialize(GLuint prog, GLfloat x, GLfloat y, GLfloat z) {
    createBuffers();
    programID = prog;
    setRotation((rand()%360)*PI*2/360, (rand()%360)*PI*2/360, (rand()%360)*PI*2/360);
    setPosition(x, y, z);
    setBuffers();   
}

void Pyramid::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}       

void Pyramid::setRotation(GLfloat a, GLfloat b, GLfloat c) {
    rot[0] = a;
    rot[1] = b;
    rot[2] = c;
}

void Pyramid::setBuffers() {
	bindBuffers();

    vertices[0] = glm::vec3(-0.86f, -0.5f, 0.41f); //dolny trójkat
    vertices[1] = glm::vec3(0.86f, -0.5f, 0.41f);
    vertices[2] = glm::vec3(0.0f,  1.0f, 0.41f);
    vertices[3] = glm::vec3(0.0f,  0.0f, -1.0f);  

    GLuint indices[] = {
        0, 1, 2,
        0, 1, 3,
        1, 2, 3,
        2, 0, 3
    };

    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, glm::vec3(pos[0], pos[1], pos[2]));
    modelmat = glm::rotate(modelmat, rot[0], glm::vec3(0.0f, 0.0f, 1.0f));
    modelmat = glm::rotate(modelmat, rot[1], glm::vec3(0.0f, 1.0f, 0.0f));
    modelmat = glm::rotate(modelmat, rot[2], glm::vec3(1.0f, 0.0f, 0.0f));
    modelmat = glm::scale(modelmat, glm::vec3(0.7, 0.7, 0.7));

    for (int i = 0; i < 4; i++) {
        glm::vec4 tmp = modelmat * glm::vec4(vertices[i].x, vertices[i].y, vertices[i].z, 1.0f);
        vertices[i] = glm::vec3(tmp.x, tmp.y, tmp.z);
    }


    glBufferData(GL_ARRAY_BUFFER, 4*sizeof(glm::vec3), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12*sizeof(GLuint), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}


void Pyramid::draw() {
	bindProgram();
    bindVAO();
    
    glm::mat4 viewmat = camera.getMainView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getMainProjection(); 
    setUniformmat4("projection", projectionmat);

    setUniform3f("col", pos[0]/(N-1), pos[1]/(N-1), pos[2]/(N-1));

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);  

    setUniform3f("col", 0, 0, 0);

    glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, (void*)0);   
}

void Pyramid::drawSecondary() {
  	bindProgram();
    bindVAO();  

    glm::mat4 viewmat = camera.getSecondaryView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getSecondaryProjection(); 
    setUniformmat4("projection", projectionmat);

    setUniform3f("col", pos[0]/(N-1), pos[1]/(N-1), pos[2]/(N-1));

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0); 

    setUniform3f("col", 0, 0, 0);

    glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, (void*)0); 
}

