#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
uniform mat4 view;
uniform mat4 projection;


void main(){

    gl_Position = projection * view * vec4(vertexPosition, 1.0);
}

