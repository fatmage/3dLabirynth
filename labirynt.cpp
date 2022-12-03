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
#include "Cube.hpp"

#include <time.h>

GLuint N = 10;
GLfloat speed = 1.0f;

Camera camera = Camera();

GLuint LoadShaders(const char * vertex_path,const char * fragment_path) {

    GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string vsCode;
    std::ifstream VertexShaderStream(vertex_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
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
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        fsCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint result = GL_FALSE;
    int InfoLogLength;


    // Compile Vertex Shader
    printf("Compiling shader: %s\n", vertex_path);
    char const *vsPointer = vsCode.c_str();
    glShaderSource(vsID, 1, &vsPointer , NULL);
    glCompileShader(vsID);

    // Check Vertex Shader
    glGetShaderiv(vsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(vsID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }



    // Compile Fragment Shader
    printf("Compiling shader: %s\n", fragment_path);
    char const * FragmentSourcePointer = fsCode.c_str();
    glShaderSource(fsID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(fsID);

    // Check Fragment Shader
    glGetShaderiv(fsID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
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
    if ( InfoLogLength > 0 ) {
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	camera.resize(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.rotateMouse(xpos, ypos);
}

glm::vec3 closestPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	const glm::vec3 ab = b - a;
	const glm::vec3 ac = c - a;
	const glm::vec3 ap = p - a;

	const GLfloat d1 = glm::dot(ab, ap);
	const GLfloat d2 = glm::dot(ac, ap);
	if (d1 <= 0.f && d2 <= 0.f) return a;

	const glm::vec3 bp = p - b;
	const GLfloat d3 = glm::dot(ab, bp);
	const GLfloat d4 = glm::dot(ac, bp);
	if (d3 >= 0.f && d4 <= d3) return b;

	const glm::vec3 cp = p - c;
	const GLfloat d5 = glm::dot(ab, cp);
	const GLfloat d6 = glm::dot(ac, cp);
	if (d6 >= 0.f && d5 <= d6) return c;

	const GLfloat vc = d1 * d4 - d3 * d2;
	if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f)
	{
		const GLfloat v = d1 / (d1 - d3);
		return a + v * ab;
	}
		
	const GLfloat vb = d5 * d2 - d1 * d6;
	if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f)
	{
		const GLfloat v = d2 / (d2 - d6);
		return a + v * ac;
	}
		
	const GLfloat va = d3 * d6 - d5 * d4;
	if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f)
	{
		const GLfloat v = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + v * (c - b);
	}

	const GLfloat denom = 1.f / (va + vb + vc);
	const GLfloat v = vb * denom;
	const GLfloat w = vc * denom;
	return a + v * ab + w * ac;
}

bool collision(glm::vec3 pos, GLfloat rad, glm::vec3 vertices[4]) {
	glm::vec3 closestPoint1 = closestPointTriangle(pos, vertices[0], vertices[1], vertices[2]);
	glm::vec3 closestPoint2 = closestPointTriangle(pos, vertices[1], vertices[2], vertices[3]);
	glm::vec3 closestPoint3 = closestPointTriangle(pos, vertices[0], vertices[2], vertices[3]);
	glm::vec3 closestPoint4 = closestPointTriangle(pos, vertices[0], vertices[1], vertices[3]);

	if (glm::length(pos - closestPoint1) <= rad || glm::length(pos - closestPoint2) <= rad || 
		glm::length(pos - closestPoint3) <= rad || glm::length(pos - closestPoint4) <= rad)
		return true;

	return false;
}

static int check(GLint a, int b) {
	return ((a + b) >= 0) && ((a + b) < N);
}


int main(int argc, char** argv)
{
	if (argc == 2) {
		N = atoi(argv[1]);
	} else if (argc == 3) {
		N = atoi(argv[1]);
		srand(atoi(argv[2]));
	} else {
		N = 10;
		srand(time(NULL));
	}
		

	// Initialise GLFW
	if ( !glfwInit() )
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
	if ( window == NULL ) {
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

	for (int i = 0; i < N; i++) {
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

	GLuint cubeProg = LoadShaders("cube.vs", "cube.fs");
	Cube cube;
	cube.initialize(cubeProg);

	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = glfwGetTime();
	GLfloat start = lastTime;


	camera.setSecondaryPosition((GLfloat)N + 3.0, (GLfloat)N + 3.0, -3.0);
	


	do{
		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glm::vec3 prevPos = player.getCenter();

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera.moveForward(speed, deltaTime);
			player.moveForward(speed, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			camera.moveBackward(speed, deltaTime);
			player.moveBackward(speed, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			camera.rotateUp(deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			camera.rotateDown(deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			camera.rotateLeft(deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			camera.rotateRight(deltaTime);
		}

		glm::vec3 playerPos = player.getCenter();
		GLfloat playerRad = player.getRadius();

		if (playerPos.x - playerRad < -0.5) {
			player.setPosition(-0.5 + playerRad, playerPos.y, playerPos.z);
			camera.setPosition(-0.5 + playerRad, playerPos.y, playerPos.z);
		}

		
		if (playerPos.x + playerRad > N-0.5) {
			player.setPosition(N-0.5 - playerRad, playerPos.y, playerPos.z);
			camera.setPosition(N-0.5 - playerRad, playerPos.y, playerPos.z);
		}


		if (playerPos.y - playerRad < -0.5) {
			player.setPosition(playerPos.x, -0.5 + playerRad, playerPos.z);
			camera.setPosition(playerPos.x, -0.5 + playerRad, playerPos.z);
		}


		if (playerPos.y + playerRad > N-0.5) {
			player.setPosition(playerPos.x, N-0.5 - playerRad, playerPos.z);
			camera.setPosition(playerPos.x, N-0.5 - playerRad, playerPos.z);
		}
			
		if (playerPos.z - playerRad < -0.5) {
			player.setPosition(playerPos.x, playerPos.y, -0.5 + playerRad);
			camera.setPosition(playerPos.x, playerPos.y, -0.5 + playerRad);
		}
			
		
		if (playerPos.z + playerRad > N-0.5) {
			player.setPosition(playerPos.x, playerPos.y, N-0.5 - playerRad);
			camera.setPosition(playerPos.x, playerPos.y, N-0.5 - playerRad);
		}

		glm::vec3 currPos = player.getCenter();

		GLint x, y, z;
		x = std::floor(currPos.x);
		y = std::floor(currPos.y);
		z = std::floor(currPos.z);

		int indices[] = {-1, 0 ,1};

		glm::vec3 vertices[4] = {
			pd[N-1][N-1][N-1].vertices[0],
			pd[N-1][N-1][N-1].vertices[1],
			pd[N-1][N-1][N-1].vertices[2],
			pd[N-1][N-1][N-1].vertices[3]
		};

		if (collision(currPos, playerRad, vertices)) {
			printf("Koniec gry, brawo!\nCzas: %f s\n", glfwGetTime() - start);
			return 0;
		}
		// check all directions
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (check(x,indices[i]) && check(y,indices[j]) && check(z,indices[k]) && (x+indices[i] != 0 || y+indices[j] != 0 || z+indices[k] != 0)) {
							vertices[0] = pd[x+indices[i]][y+indices[j]][z+indices[k]].vertices[0];
							vertices[1] = pd[x+indices[i]][y+indices[j]][z+indices[k]].vertices[1];
							vertices[2] = pd[x+indices[i]][y+indices[j]][z+indices[k]].vertices[2];
							vertices[3] = pd[x+indices[i]][y+indices[j]][z+indices[k]].vertices[3];
						if (collision(currPos, playerRad, vertices)) {
							i = 3;
							j = 3;
							k = 3;
							camera.setPosition(prevPos.x, prevPos.y, prevPos.z);
							player.setPosition(prevPos.x, prevPos.y, prevPos.z);
						}

					}
				}
			}
		}
			

		// Clear the screen
		camera.setMainViewport();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.draw();
		
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				for (int k = 0; k < N; k++) {
					if (!(i == 0 && j == 0 && k == 0))
						pd[i][j][k].draw();
				}
			}
		}
		
		player.draw();

		camera.setSecondaryViewport();

		glClear(GL_DEPTH_BUFFER_BIT);

		cube.drawSecondary();

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				for (int k = 0; k < N; k++) {
					if (!(i == 0 && j == 0 && k == 0))
						pd[i][j][k].drawSecondary();
				}
			}
		}
		
		player.drawSecondary();


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

