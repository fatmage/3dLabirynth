#version 330 core

// Ouput data
out vec4 color;
in vec3 pos;

void main()
{

	// Output color = red 
	color = vec4(pos.xyz, 1.0);

}