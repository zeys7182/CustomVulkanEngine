#include "Scene.h"

void Scene::addObject(Model* model, const glm::mat4& transform) {
    objects.push_back({model, transform});
}

void Scene::clear() {
    objects.clear();
}

const std::vector<SceneObject>& Scene::getObjects() const {
    return objects;
}