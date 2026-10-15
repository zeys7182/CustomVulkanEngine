#pragma once

#include <glm/mat4x4.hpp>

#include <vector>

class Scene {
public:
    void addObject(const glm::mat4& transform);
    void clear();

    const std::vector<glm::mat4>& getObjectTransforms() const;

private:
    std::vector<glm::mat4> objectTransforms;
};