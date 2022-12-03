#version 330 core

// Ouput data
out vec4 color;
uniform vec3 col;

void main()
{

	// Output color = red 
	color = vec4(col.xyz, 1.0);

}