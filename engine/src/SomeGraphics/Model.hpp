#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "glm/ext/vector_float3.hpp"

namespace sg {

class Mesh;

class Model {
public:
    static std::optional<std::unique_ptr<Model>> load(const char* filename);
    Model() = delete;
    Model(std::vector<Mesh>&& meshes);
    Model(Model&& other) = default;
    Model(const Model& other) = delete;
    Model& operator=(Model&& other) = default;
    Model& operator=(const Model& other) = delete;
    ~Model();

    const std::vector<Mesh>& meshes() const;
private:
    std::vector<Mesh> m_meshes;

    Model(const aiScene* scene);

    void process_node(aiNode* node, const aiScene* scene);
    void process_mesh(aiMesh* mesh);

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;

        Vertex() = delete;
        Vertex(const glm::vec3& position, const glm::vec3& normal) :
            position(position),
            normal(normal)
        {
        }
        Vertex(Vertex&& other) = default;
        Vertex(const Vertex& other) = default;
        Vertex& operator=(Vertex&& other) = default;
        Vertex& operator=(const Vertex& other) = default;
        ~Vertex() = default;
    };
};

}
