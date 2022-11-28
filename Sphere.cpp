#include "Sphere.hpp"
#include <iostream>
#include <vector>

extern GLuint LoadShaders(const char * vertex_path,const char * fragment_path);



Sphere::Sphere(const char * vertex_path,const char * fragment_path) {
    createBuffers();
    programID = LoadShaders(vertex_path, fragment_path);
    setBuffers();
}

Sphere::Sphere() {
}

void Sphere::initialize(GLuint prog) {
    radius = 0.5;
    createBuffers();
    programID = prog;
    setBuffers();   
    setPosition(0.0, 0.0, 0.0);
    setRotation(0.0, 0.0, 0.0);

}

void Sphere::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    if (x >= 0)
        pos[0] = x;
    if (y >= 0)
        pos[1] = y;
    if (z >= 0)
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

    //for (int i = 0; i < vertices.size(); i+= 3)
      //  printf("%f %f %f\n", vertices[i], vertices[i+1], vertices[i+2]);

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
            if(i != (STACKS-1))
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

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

}


void Sphere::draw() {
	bindProgram();
    bindVAO();

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}



