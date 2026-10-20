#include "Scene.h"
#include <cmath>
#include <algorithm>

void Scene::addObject(Model* model, const Transform& transform, const Material& material, const glm::vec3& angularVelocityDegrees) {
    objects.push_back({model, transform, material, angularVelocityDegrees});
}

void Scene::update(float deltaTime) {
    for (SceneObject& object : objects) {
        object.transform.rotationDegrees += object.angularVelocityDegrees * deltaTime;

        for (int axis = 0; axis < 3; ++axis) {
            object.transform.rotationDegrees[axis] = std::fmod(object.transform.rotationDegrees[axis], 360.0f);
        }
    }
}

void Scene::clear() {
    objects.clear();
    staticColliders.clear();
}

const std::vector<SceneObject>& Scene::getObjects() const {
    return objects;
}

void Scene::addStaticCollider(const AABB& collider) {
    staticColliders.push_back(collider);
}

glm::vec3 Scene::resolveMovement(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec3& displacement) const {
    glm::vec3 position = center;

    for (int axis : {0, 2}) {
        const int otherAxis = 2 - axis;
        float target = position[axis] + displacement[axis];

        for (const AABB& collider : staticColliders) {
            const glm::vec3 expandedMinimum = collider.minimum - halfExtents;
            const glm::vec3 expandedMaximum = collider.maximum + halfExtents;

            if (position.y <= expandedMinimum.y || position.y >= expandedMaximum.y) {
                continue;
            }

            if (position[otherAxis] <= expandedMinimum[otherAxis] || position[otherAxis] >= expandedMaximum[otherAxis]) {
                continue;
            }

            if (displacement[axis] > 0.0f && position[axis] <= expandedMinimum[axis]) {
                target = std::min(target, expandedMinimum[axis]);
            }

            if (displacement[axis] < 0.0f && position[axis] >= expandedMaximum[axis]) {
                target = std::max(target, expandedMaximum[axis]);
            }
        }

        position[axis] = target;
    }

    return position;
}

bool Scene::raycastStatic(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, float& hitDistance) const {
    bool hit = false;
    float closestDistance = maxDistance;

    for (const AABB& collider : staticColliders) {
        float distance = 0.0f;

        if (collider.intersectRay(origin, direction, closestDistance, distance)) {
            closestDistance = distance;
            hit = true;
        }
    }

    if (hit) {
        hitDistance = closestDistance;
    }

    return hit;
}

void Scene::addTarget(Model* model, const Transform& transform, const Material& material) {
    SceneObject object{};
    object.model = model;
    object.transform = transform;
    object.material = material;
    object.isTarget = true;
    object.initialTargetColor = material.baseColor;

    objects.push_back(object);

    AABB bounds{};
    bounds.minimum = transform.position - transform.scale * 0.5f;
    bounds.maximum = transform.position + transform.scale * 0.5f;

    addStaticCollider(bounds);
}

bool Scene::shootTarget(const glm::vec3& origin, const glm::vec3& direction, float maxDistance) {
    float closestDistance = maxDistance;
    raycastStatic(origin, direction, maxDistance, closestDistance);

    SceneObject* hitTarget = nullptr;

    for (SceneObject& object : objects) {
        if (!object.isTarget) {
            continue;
        }

        AABB bounds{};
        bounds.minimum = object.transform.position - object.transform.scale * 0.5f;
        bounds.maximum = object.transform.position + object.transform.scale * 0.5f;

        float distance = 0.0f;

        if (bounds.intersectRay(origin, direction, closestDistance, distance)) {
            closestDistance = distance;
            hitTarget = &object;
        }
    }

    if (hitTarget == nullptr || hitTarget->wasHit) {
        return false;
    }

    hitTarget->wasHit = true;
    hitTarget->material.baseColor = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f);
    return true;
}

std::size_t Scene::getTargetCount() const {
    std::size_t count = 0;

    for (const SceneObject& object : objects) {
        if (object.isTarget) {
            ++count;
        }
    }

    return count;
}

std::size_t Scene::getHitTargetCount() const {
    std::size_t count = 0;

    for (const SceneObject& object : objects) {
        if (object.isTarget && object.wasHit) {
            ++count;
        }
    }

    return count;
}

void Scene::resetTargets() {
    for (SceneObject& object : objects) {
        if (object.isTarget) {
            object.wasHit = false;
            object.material.baseColor = object.initialTargetColor;
        }
    }
}