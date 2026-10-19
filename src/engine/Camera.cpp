#include "Camera.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() = default;

void Camera::processInput(GLFWwindow* window, float deltaTime) {
    const float movementSpeed = 2.5f;
    const glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

    glm::vec3 forward = getFront();
    forward.y = 0.0f;
    forward = glm::normalize(forward);

    const glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));

    glm::vec3 movement{0.0f};

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movement += forward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movement -= forward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movement -= right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movement += right;

    if (glm::dot(movement, movement) > 0.0f) {
        movement = glm::normalize(movement);
        position += movement * movementSpeed * deltaTime;
    }
}

void Camera::constrainPosition(const glm::vec3& minimum, const glm::vec3& maximum) {
    position.x = std::clamp(position.x, minimum.x, maximum.x);
    position.y = std::clamp(position.y, minimum.y, maximum.y);
    position.z = std::clamp(position.z, minimum.z, maximum.z);
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