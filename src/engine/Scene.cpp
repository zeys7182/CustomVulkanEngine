#include "Scene.h"

void Scene::addObject(const glm::mat4& transform) {
    objectTransforms.push_back(transform);
}

void Scene::clear() {
    objectTransforms.clear();
}

const std::vector<glm::mat4>& Scene::getObjectTransforms() const {
    return objectTransforms;
}