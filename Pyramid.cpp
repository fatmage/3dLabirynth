#include "Pyramid.hpp"
#include <iostream>

extern GLuint LoadShaders(const char * vertex_path,const char * fragment_path);


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

    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::scale(modelmat, glm::vec3(0.7/N, 0.7/N, 0.7/N));
    modelmat = glm::rotate(modelmat, rot[0], glm::vec3(0.0f, 0.0f, 1.0f));
    modelmat = glm::rotate(modelmat, rot[1], glm::vec3(0.0f, 1.0f, 0.0f));
    modelmat = glm::rotate(modelmat, rot[2], glm::vec3(1.0f, 0.0f, 0.0f));
    modelmat = glm::translate(modelmat, glm::vec3(pos[0], pos[1], pos[2]));
    setUniformmat4("model", modelmat);

    glm::mat4 viewmat = glm::lookAt(
        glm::vec3(-10, -10, -10),
        glm::vec3(0.0, 0.0, 0.0),
        glm::vec3(0,1,0)
    );
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = glm::perspective<float>(
        glm::radians((90.0/360)*2*PI), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        3.0f / 3.0f,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f             // Far clipping plane. Keep as little as possible.
    );

    setUniformmat4("projection", projectionmat);

    setUniform3f("colorr", pos[0]/(N-1), pos[1]/(N-1), pos[2]/(N-1));

    glDrawArrays(GL_TRIANGLES, 0, 12);   

    setUniform3f("colorr", 0, 0, 0);

    glDrawArrays(GL_LINES, 0, 12);   
}

