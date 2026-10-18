#pragma once

#include "Material.h"

#include <glm/mat4x4.hpp>
#include <vector>

class Model;

struct SceneObject {
    Model* model = nullptr;
    glm::mat4 transform{1.0f};
    Material material{};
};

class Scene {
public:
    void addObject(Model* model, const glm::mat4& transform, const Material& material = Material{});
    void clear();

    const std::vector<SceneObject>& getObjects() const;

private:
    std::vector<SceneObject> objects;
};