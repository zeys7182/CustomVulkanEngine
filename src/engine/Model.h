#pragma once

#include "Mesh.h"

#include <memory>
#include <string>

class Model {
public:
    Model(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, const std::string& path);

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void bind(VkCommandBuffer commandBuffer) const;
    void draw(VkCommandBuffer commandBuffer) const;

private:
    void loadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices);

private:
    std::unique_ptr<Mesh> mesh;
};