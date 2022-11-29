#include "Camera.hpp"

extern GLFWwindow *window;



Camera::Camera() {
    setMainWindow(600,600);
    setSecondaryWindow(100,100);

    cameraPos = glm::vec3(-1.0f, -1.0f, -1.0f);
    cameraFront = glm::vec3(1.0f, 1.0f, 1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

}

void Camera::resize(int w, int h) {
    setMainWindow(w, h);
	setSecondaryWindow(w, h);
}

void Camera::setMainWindow(int w, int h) {
    mainWindowWidth = w;
    mainWindowHeight = h;
}

void Camera::setSecondaryWindow(int w, int h) {
    secondaryWindowWidth = w < 200 ? w/2 : 100;
    secondaryWindowHeight = h < 200 ? h/2 : 100;
}


int Camera::getMainWidth() {
    return mainWindowWidth;
}

int Camera::getMainHeight() {
    return mainWindowHeight;
}

int Camera::getSecondaryWidth() {
    return secondaryWindowWidth;
}

int Camera::getSecondaryHeight() {
    return secondaryWindowHeight;
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(
        glm::radians(40.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        (float)mainWindowWidth / (float)mainWindowHeight,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.1f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f             // Far clipping plane. Keep as little as possible.
    );
}


glm::mat4 Camera::getMainView() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::moveForward() {
    cameraPos += cameraSpeed * cameraFront;
}

void Camera::moveBackward() {
    cameraPos -= cameraSpeed * cameraFront;
}

void Camera::moveLeft() {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::moveRight() {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


