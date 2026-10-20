#pragma once

#include "Material.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include "Transform.h"
#include "AABB.h"

class Model;

struct SceneObject {
    Model* model = nullptr;
    Transform transform{};
    Material material{};
    glm::vec3 angularVelocityDegrees{0.0f};
    bool isTarget = false;
};

class Scene {
public:
    void addObject(Model* model, const Transform& transform, const Material& material = Material{}, const glm::vec3& angularVelocityDegrees = glm::vec3(0.0f));
    void update(float deltaTime);
    void clear();

    const std::vector<SceneObject>& getObjects() const;

    void addStaticCollider(const AABB& collider);
    glm::vec3 resolveMovement(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec3& displacement) const;
    bool raycastStatic(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, float& hitDistance) const;

    void addTarget(Model* model, const Transform& transform, const Material& material);
    bool shootTarget(const glm::vec3& origin, const glm::vec3& direction, float maxDistance);

private:
    std::vector<SceneObject> objects;
    std::vector<AABB> staticColliders;
};