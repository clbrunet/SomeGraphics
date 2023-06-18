#pragma once

#include <optional>
#include <vector>

#include "assimp/mesh.h"
#include "assimp/scene.h"

#include "SomeGraphics/Mesh.hpp"

namespace sg {

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
};

}
