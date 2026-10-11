#include "Camera.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() = default;

void Camera::processInput(GLFWwindow* window, float deltaTime) {
    const float cameraSpeed = 2.5f * deltaTime;
    const glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
    const glm::vec3 forward = getFront();
    const glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += forward * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position -= forward * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position -= right * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += right * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) position += worldUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) position -= worldUp * cameraSpeed;
}

void Camera::processMouseMovement(GLFWwindow* window) {
    double mouseX = 0.0;
    double mouseY = 0.0;

    glfwGetCursorPos(window, &mouseX, &mouseY);

    if (firstMouse) {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouse = false;
    }

    const float xOffset = static_cast<float>(mouseX - lastMouseX);
    const float yOffset = static_cast<float>(lastMouseY - mouseY);

    lastMouseX = mouseX;
    lastMouseY = mouseY;

    const float sensitivity = 0.1f;
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;
    pitch = std::clamp(pitch, -89.0f, 89.0f);
}

glm::vec3 Camera::getFront() const {
    glm::vec3 front{};
    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    return glm::normalize(front);
}

glm::mat4 Camera::getViewMatrix() const {
    const glm::vec3 worldUp{0.0f, 1.0f, 0.0f};
    return glm::lookAt(position, position + getFront(), worldUp);
}