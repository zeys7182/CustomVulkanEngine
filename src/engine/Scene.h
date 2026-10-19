#pragma once

#include "Material.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include "Transform.h"

class Model;

struct SceneObject {
    Model* model = nullptr;
    Transform transform{};
    Material material{};
    glm::vec3 angularVelocityDegrees{0.0f};
};

class Scene {
public:
    void addObject(Model* model, const Transform& transform, const Material& material = Material{}, const glm::vec3& angularVelocityDegrees = glm::vec3(0.0f));
    void update(float deltaTime);
    void clear();

    const std::vector<SceneObject>& getObjects() const;

private:
    std::vector<SceneObject> objects;
};