#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
out vec3 colorr;
uniform mat4 transform;
uniform mat4 model;

void main(){

    gl_Position = transform * vec4(vertexPosition.xyz, 1.0);
    gl_Position = model * gl_Position;
    colorr = vertexPosition;
}

