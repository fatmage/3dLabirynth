#ifndef CAMERA_HPP
#define CAMERA_HPP

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <stdlib.h>
#include <string.h>

class Camera {
    private:
    int mainWindowWidth, mainWindowHeight;
    int secondaryWindowWidth, secondaryWindowHeight;
    float pitch, yaw;

    float lastX, lastY;

    public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
	float cameraSpeed = 0.05f;

    Camera();
    glm::mat4 getMainView();
    glm::mat4 getSecondaryView();
    glm::mat4 getProjection();
    void setMainWindow(int w, int h);
    void setSecondaryWindow(int w, int h);
    int getMainWidth();
    int getMainHeight();
    int getSecondaryWidth();
    int getSecondaryHeight();
    void resize(int w, int h);
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void rotate(float xpos, float ypos);

};

#endif