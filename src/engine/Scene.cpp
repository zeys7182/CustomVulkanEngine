#include "Scene.h"

void Scene::addObject(Model* model, const Transform& transform, const Material& material) {
    objects.push_back({model, transform, material});
}

void Scene::clear() {
    objects.clear();
}

const std::vector<SceneObject>& Scene::getObjects() const {
    return objects;
}