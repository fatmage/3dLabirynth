#include "Camera.hpp"

extern GLFWwindow *window;



Camera::Camera() {
    setMainWindow(600,600);
    lastX = 300;
    lastY = 300;
    setSecondaryWindow(100,100);
    pitch = 45.0f;
    yaw = 45.0f;

    cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraFront = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));/*glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                            sin(glm::radians(pitch)),
                            sin(glm::radians(yaw)) * cos(glm::radians(pitch)));*/
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
        glm::radians(45.0f), // The vertical Field of View, in radians: the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
        (float)mainWindowWidth / (float)mainWindowHeight,       // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
        0.8f,              // Near clipping plane. Keep as big as possible, or you'll get precision issues.
        100.0f             // Far clipping plane. Keep as little as possible.
    );
}


glm::mat4 Camera::getMainView() {
    return glm::lookAt(cameraPos+cameraFront, cameraPos, cameraUp);
}

void Camera::moveForward() {
    cameraPos -= cameraSpeed * cameraFront;
}

void Camera::moveBackward() {

    cameraPos += cameraSpeed * cameraFront;
}

void Camera::moveLeft() {
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::moveRight() {
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::rotate(float xpos, float ypos) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y ranges bottom to top
    lastX = xpos;
    lastY = ypos;
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch -= yoffset;

    if(pitch > 89.0f)
    pitch = 89.0f;
    if(pitch < -89.0f)
    pitch = -89.0f;

    glm::vec3 direction;

    cameraFront = glm::normalize(glm::vec3( cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                                            sin(glm::radians(pitch)),
                                            sin(glm::radians(yaw)) * cos(glm::radians(pitch))));

    printf("%f %f\n", pitch, yaw);

}





