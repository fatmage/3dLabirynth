#version 330 core

// Ouput data
out vec4 color;
uniform vec3 colorr;

void main()
{

	// Output color = red 
	color = vec4(colorr.xyz, 1.0);

}