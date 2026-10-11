#pragma once

#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
    Camera();

    void processInput(GLFWwindow* window, float deltaTime);
    void processMouseMovement(GLFWwindow* window);

    glm::mat4 getViewMatrix() const;
    glm::vec3 getFront() const;

private:
    glm::vec3 position{2.0f, 2.0f, 2.0f};
    float yaw = -135.0f;
    float pitch = -25.0f;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
};