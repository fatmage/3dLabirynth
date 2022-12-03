#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
uniform mat4 view;
uniform mat4 projection;
varying vec3 worldPos;
varying vec3 localPos;



void main(){
    worldPos = vertexPosition;
    gl_Position = projection * view * vec4(vertexPosition, 1.0);
    localPos = gl_Position.xyz;
}

