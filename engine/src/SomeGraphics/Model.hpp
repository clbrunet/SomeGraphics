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
    Model(std::vector<Mesh>&& meshes);
    ~Model();

    const std::vector<Mesh>& meshes() const;
private:
    std::vector<Mesh> m_meshes;

    Model(const aiScene* scene);
    void process_node(aiNode* node, const aiScene* scene);
    void process_mesh(aiMesh* mesh);
};

}
