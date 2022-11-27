#version 330 core

// Ouput data
out vec4 color;
in vec3 colorr;

void main()
{

	// Output color = red 
	color = vec4((colorr.x+1.0)/2,(colorr.y+1.0)/2,(colorr.z+1.0)/2, 1.0);

}