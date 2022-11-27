// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.hpp>
#include "Pyramid.hpp"

#include <time.h>

GLuint N = 10;




int main( void )
{

	srand(time(NULL));
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 650, 650, "Tutorial 02 - Red triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, 650, 650);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_DEPTH_TEST);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	// Create and compile our GLSL program from the shaders
	Program pyramidProg = Program("pyramid.vs", "pyramid.fs");
	Pyramid pd[N][N][N];
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				pd[i][j][k].initialize(pyramidProg);
				pd[i][j][k].setPosition((float)i/N, (float)j/N, (float)i/N);
			}
		}
	}

	//Program playerProg = Program("sphere.vs", "sphere.fs");



	do{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				for (int k = 0; k < N; k++)
					pd[i][j][k].draw();
			}
		}
		

		GLenum er;
		while (er=glGetError())
			fprintf(stderr, "\nOpenGL ERROR: 0x%04x    =============%s===\n", er, "main afterdraw");

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

