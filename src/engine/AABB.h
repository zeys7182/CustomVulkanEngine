#pragma once

#include <glm/vec3.hpp>

struct AABB {
    glm::vec3 minimum{0.0f};
    glm::vec3 maximum{0.0f};
};