#include "Sphere.hpp"
#include "Camera.hpp"
#include <iostream>
#include <vector>

extern GLuint LoadShaders(const char * vertex_path,const char * fragment_path);

extern GLuint N;

extern Camera camera;


Sphere::Sphere(const char * vertex_path,const char * fragment_path) {
    createBuffers();
    programID = LoadShaders(vertex_path, fragment_path);
    setBuffers();
}

Sphere::Sphere() {
}

void Sphere::initialize(GLuint prog) {
    radius = 1.0;
    createBuffers();
    programID = prog;
    setPosition(0.0, 0.0, 0.0);
    setRotation((rand()%360)*PI*2/360, (rand()%360)*PI*2/360, (rand()%360)*PI*2/360);
    setBuffers();   
}

void Sphere::setPosition(GLfloat x, GLfloat y, GLfloat z) {
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
}       

void Sphere::setRotation(GLfloat a, GLfloat b, GLfloat c) {
    rot[0] = a;
    rot[1] = b;
    rot[2] = c;
}

void Sphere::createBuffers() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

void Sphere::setBuffers() {
    bindVAO();
	bindBuffers();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


    GLfloat x, y, z, xy;                              // vertex position
    GLfloat nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    GLfloat s, t;      

    GLfloat sectorStep = 2*PI/ SECTORS;
    GLfloat stackStep = PI / STACKS;
    GLfloat sectorAngle, stackAngle;

    
    for (int i = 0; i <= STACKS; ++i) {
        stackAngle = (PI / 2) - i * stackStep;
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= SECTORS; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / SECTORS;
            t = (float)i / STACKS;
            texCoords.push_back(s);
            texCoords.push_back(t);
        }
    }

    int k1, k2;
    for(int i = 0; i < STACKS; ++i)
    {
        k1 = i * (SECTORS + 1);     // beginning of current stack
        k2 = k1 + SECTORS + 1;      // beginning of next stack

        for(int j = 0; j < SECTORS; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (STACKS))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


}


void Sphere::draw() {
	bindProgram();
    bindVAO();


    glm::mat4 modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, glm::vec3(pos[0], pos[1], pos[2]));
    modelmat = glm::rotate(modelmat, rot[0], glm::vec3(0.0f, 0.0f, 1.0f));
    modelmat = glm::rotate(modelmat, rot[1], glm::vec3(0.0f, 1.0f, 0.0f));
    modelmat = glm::rotate(modelmat, rot[2], glm::vec3(1.0f, 0.0f, 0.0f));
    modelmat = glm::scale(modelmat, glm::vec3(0.15, 0.15, 0.15));
    setUniformmat4("model", modelmat);

    glm::mat4 viewmat = camera.getMainView();
    setUniformmat4("view", viewmat);

    glm::mat4 projectionmat = camera.getProjection();
    setUniformmat4("projection", projectionmat);


    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}


