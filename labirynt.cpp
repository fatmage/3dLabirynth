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

#include "DrawableObject.hpp"
#include "Pyramid.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"

#include <time.h>

GLuint N = 10;

Camera camera = Camera();

GLuint LoadShaders(const char * vertex_path,const char * fragment_path){

    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vsCode;
    std::ifstream VertexShaderStream(vertex_path, std::ios::in);
    if(VertexShaderStream.is_open()){
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        vsCode = sstr.str();
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_path);
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string fsCode;
    std::ifstream FragmentShaderStream(fragment_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        fsCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_path);
    char const *vsPointer = vsCode.c_str();
    glShaderSource(vsID, 1, &vsPointer , NULL);
    glCompileShader(vsID);

    // Check Vertex Shader
    glGetShaderiv(vsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(vsID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_path);
    char const * FragmentSourcePointer = fsCode.c_str();
    glShaderSource(fsID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(fsID);

    // Check Fragment Shader
    glGetShaderiv(fsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(fsID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }




    GLuint programID = glCreateProgram();
    glAttachShader(programID, vsID);
    glAttachShader(programID, fsID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    
    glDetachShader(programID, vsID);
    glDetachShader(programID, fsID);
    
    glDeleteShader(vsID);
    glDeleteShader(fsID);

    return programID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	camera.resize(width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.rotate(xpos, ypos);
}

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
	window = glfwCreateWindow(camera.getMainWidth(), camera.getMainHeight(), "Tutorial 02 - Red triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glViewport(0, 0, camera.getMainWidth(), camera.getMainHeight());
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



	

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	// Create and compile our GLSL program from the shaders
	GLuint pyramidProg = LoadShaders("pyramid.vs", "pyramid.fs");
	Pyramid pd[N][N][N];

	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				pd[i][j][k].initialize(pyramidProg, i, j, k);
			}
		}
	}

	GLuint sphereProg = LoadShaders("sphere.vs", "sphere.fs");
	Sphere player;
	player.initialize(sphereProg);
	player.setPosition(0.0, 0.0, 0.0);


	do{

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			camera.moveForward();
			player.moveForward();
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera.moveBackward();
			player.moveBackward();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.moveLeft();
			player.moveLeft();
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera.moveRight();
			player.moveRight();
		}

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				for (int k = 0; k < N; k++) {
					if (!(i == 0 && j == 0 && k == 0))
						pd[i][j][k].draw();
				}
			}
		}
		
		player.draw();


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

