#pragma once

#include <glm/vec3.hpp>

struct AABB {
    glm::vec3 minimum{0.0f};
    glm::vec3 maximum{0.0f};

    bool intersectRay(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, float& hitDistance) const {
        float nearDistance = 0.0f;
        float farDistance = maxDistance;

        for (int axis = 0; axis < 3; ++axis) {
            if(std::abs(direction[axis]) < 0.000001f) {
                if(origin[axis] < minimum[axis] || origin[axis] > maximum[axis]) {
                    return false;
                }

                continue;
            }

            float entry = (minimum[axis] - origin[axis]) / direction[axis];
            float exit = (maximum[axis] - origin[axis]) / direction[axis];

            if (entry > exit) {
                std::swap(entry, exit);
            }
            
            nearDistance = std::max(nearDistance, entry);
            farDistance = std::min(farDistance, exit);

            if (nearDistance > farDistance) {
                return false;
            }
        }

        hitDistance = nearDistance;
        return true;
    }
};