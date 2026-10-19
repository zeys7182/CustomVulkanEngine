#include "Scene.h"
#include <cmath>

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
}

const std::vector<SceneObject>& Scene::getObjects() const {
    return objects;
}