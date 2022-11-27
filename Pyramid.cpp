#include "Pyramid.hpp"
#include <iostream>


Pyramid::Pyramid(Program prog) {
    createBuffers();
    setProgram(prog);
    setBuffers();
}

Pyramid::Pyramid() {
}

void Pyramid::initialize(Program prog) {
    createBuffers();
    setProgram(prog);
    setBuffers();   
    rot[0] = rand()%7;
    rot[1] = rand()%7;
    rot[2] = rand()%7;
}

void Pyramid::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    if (x >= 0)
        pos[0] = x;
    if (y >= 0)
        pos[1] = y;
    if (z >= 0)
        pos[2] = z;
}       

void Pyramid::setRotation(GLfloat a, GLfloat b, GLfloat c) {
    rot[0] = a;
    rot[1] = b;
    rot[2] = c;
}

void Pyramid::setBuffers() {
	bindBuffers();

    GLfloat vertices[] = { 
        -0.86f, -0.5f, 0.41f, //dolny trójkat
        0.86f, -0.5f, 0.41f,
        0.0f,  1.0f, 0.41f,
        
        -0.86f, -0.5f, 0.41f,
        0.86f, -0.5f, 0.41f,
        0.0f,  0.0f, -1.0f,

        0.86f, -0.5f, 0.41f,
        0.0f,  1.0f, 0.41f,
        0.0f,  0.0f, -1.0f,

        -0.86f, -0.5f, 0.41f,
        0.0f,  1.0f, 0.41f,
        0.0f,  0.0f, -1.0f,
            
    };



    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}


void Pyramid::draw() {
	bindProgram();
    bindVAO();

    glm::mat4 transformmat = glm::mat4(1.0f);
    transformmat = glm::scale(transformmat, glm::vec3(0.7/N, 0.7/N, 0.7/N));
    transformmat = glm::rotate(transformmat, rot[0], glm::vec3(0.0f, 0.0f, 1.0f));
    transformmat = glm::rotate(transformmat, rot[1], glm::vec3(0.0f, 1.0f, 0.0f));
    transformmat = glm::rotate(transformmat, rot[2], glm::vec3(1.0f, 0.0f, 0.0f));
    setUniformmat4("transform", transformmat);

    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, glm::vec3(pos[0], pos[1], pos[2]));
    setUniformmat4("model", modelmat);

    glDrawArrays(GL_TRIANGLES, 0, 12);   
}



