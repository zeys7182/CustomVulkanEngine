#include "Model.h"

#include <array>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
    struct VertexKey {
        int position = 0;
        int texCoord = 0;
        int normal = 0;

        bool operator==(const VertexKey& other) const {
            return position == other.position && texCoord == other.texCoord && normal == other.normal;
        }
    };

    struct VertexKeyHash {
        std::size_t operator()(const VertexKey& key) const {
            std::size_t result = std::hash<int>{}(key.position);
            result ^= std::hash<int>{}(key.texCoord) << 1;
            result ^= std::hash<int>{}(key.normal) << 2;
            return result;
        }
    };
}

Model::Model(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, const std::string& path) {
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    loadObj(path, vertices, indices);
    mesh = std::make_unique<Mesh>(device, physicalDevice, commandPool, graphicsQueue, vertices, indices);
}

void Model::bind(VkCommandBuffer commandBuffer) const {
    mesh->bind(commandBuffer);
}

void Model::draw(VkCommandBuffer commandBuffer) const {
    mesh->draw(commandBuffer);
}

void Model::loadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open model file: " + path);
    }

    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<float, 2>> texCoords;
    std::vector<std::array<float, 3>> normals;
    std::unordered_map<VertexKey, uint16_t, VertexKeyHash> uniqueVertices;

    auto getVertexIndex = [&](const VertexKey& key) -> uint16_t {
        auto existingVertex = uniqueVertices.find(key);

        if (existingVertex != uniqueVertices.end()) {
            return existingVertex->second;
        }

        if (key.position <= 0 || key.position > static_cast<int>(positions.size())) {
            throw std::runtime_error("Invalid position index in OBJ file");
        }

        if (key.texCoord <= 0 || key.texCoord > static_cast<int>(texCoords.size())) {
            throw std::runtime_error("Invalid texture coordinate index in OBJ file");
        }

        if (key.normal <= 0 || key.normal > static_cast<int>(normals.size())) {
            throw std::runtime_error("Invalid normal index in OBJ file");
        }

        if (vertices.size() >= 65535) {
            throw std::runtime_error("OBJ model has too many vertices for uint16 indices");
        }

        Vertex vertex{};
        vertex.position[0] = positions[key.position - 1][0];
        vertex.position[1] = positions[key.position - 1][1];
        vertex.position[2] = positions[key.position - 1][2];
        vertex.color[0] = 1.0f;
        vertex.color[1] = 1.0f;
        vertex.color[2] = 1.0f;
        vertex.texCoord[0] = texCoords[key.texCoord - 1][0];
        vertex.texCoord[1] = texCoords[key.texCoord - 1][1];
        vertex.normal[0] = normals[key.normal - 1][0];
        vertex.normal[1] = normals[key.normal - 1][1];
        vertex.normal[2] = normals[key.normal - 1][2];

        const uint16_t newIndex = static_cast<uint16_t>(vertices.size());
        vertices.push_back(vertex);
        uniqueVertices[key] = newIndex;

        return newIndex;
    };

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix.empty() || prefix[0] == '#') {
            continue;
        }

        if (prefix == "v") {
            std::array<float, 3> position{};
            lineStream >> position[0] >> position[1] >> position[2];
            positions.push_back(position);
        } else if (prefix == "vt") {
            std::array<float, 2> texCoord{};
            lineStream >> texCoord[0] >> texCoord[1];
            texCoords.push_back(texCoord);
        } else if (prefix == "vn") {
            std::array<float, 3> normal{};
            lineStream >> normal[0] >> normal[1] >> normal[2];
            normals.push_back(normal);
        } else if (prefix == "f") {
            std::vector<VertexKey> face;
            std::string faceVertex;

            while (lineStream >> faceVertex) {
                std::istringstream vertexStream(faceVertex);
                std::string index;
                VertexKey key{};

                std::getline(vertexStream, index, '/');
                key.position = std::stoi(index);

                std::getline(vertexStream, index, '/');
                key.texCoord = std::stoi(index);

                std::getline(vertexStream, index, '/');
                key.normal = std::stoi(index);

                face.push_back(key);
            }

            if (face.size() < 3) {
                throw std::runtime_error("OBJ face has fewer than three vertices");
            }

            for (std::size_t i = 1; i + 1 < face.size(); ++i) {
                indices.push_back(getVertexIndex(face[0]));
                indices.push_back(getVertexIndex(face[i]));
                indices.push_back(getVertexIndex(face[i + 1]));
            }
        }
    }

    if (vertices.empty() || indices.empty()) {
        throw std::runtime_error("OBJ model contains no geometry: " + path);
    }
}