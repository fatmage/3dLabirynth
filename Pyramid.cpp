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

void Pyramid::initialize(GLuint prog) {
    createBuffers();
    programID = prog;
    setBuffers();   
    rot[0] = (rand()%360)*PI*2/360;
    rot[1] = (rand()%360)*PI*2/360;
    rot[2] = (rand()%360)*PI*2/360;
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

    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, glm::vec3(pos[0], pos[1], pos[2]));
    modelmat = glm::rotate(modelmat, rot[0], glm::vec3(0.0f, 0.0f, 1.0f));
    modelmat = glm::rotate(modelmat, rot[1], glm::vec3(0.0f, 1.0f, 0.0f));
    modelmat = glm::rotate(modelmat, rot[2], glm::vec3(1.0f, 0.0f, 0.0f));
    modelmat = glm::scale(modelmat, glm::vec3(0.7, 0.7, 0.7));


    setUniformmat4("model", modelmat);

    glm::mat4 viewmat = camera.getMainView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getProjection(); 
    setUniformmat4("projection", projectionmat);

    setUniform3f("colorr", pos[0]/(N-1), pos[1]/(N-1), pos[2]/(N-1));

    glDrawArrays(GL_TRIANGLES, 0, 12);   

    setUniform3f("colorr", 0, 0, 0);

    glDrawArrays(GL_LINES, 0, 12);   
}

