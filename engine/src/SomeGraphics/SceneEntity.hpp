#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

namespace sg {

class Mesh;
class Texture;

class SceneEntity {
public:
    static std::optional<std::shared_ptr<SceneEntity>> load_model(const char* filename);
    SceneEntity() = delete;
    SceneEntity(SceneEntity&& other) = default;
    SceneEntity(const SceneEntity& other) = delete;
    SceneEntity& operator=(SceneEntity&& other) = default;
    SceneEntity& operator=(const SceneEntity& other) = delete;
    ~SceneEntity() = default;

    const glm::mat4& transform() const;
    const std::unique_ptr<Mesh>& mesh() const;
    const std::shared_ptr<Texture>& texture() const;
    const std::vector<std::shared_ptr<SceneEntity>>& children() const;
private:
    glm::mat4 m_transform = glm::mat4(1.0f);
    std::unique_ptr<Mesh> m_mesh;
    std::shared_ptr<Texture> m_texture;
    std::vector<std::shared_ptr<SceneEntity>> m_children;

    SceneEntity(const std::string& filename, const aiNode* node, const aiMatrix4x4& transform,
        const aiScene* scene);

    void process_node_meshes(const std::string& filename, const aiNode* node, const aiScene* scene);

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture_coordinates;

        Vertex() = delete;
        Vertex(const glm::vec3& position, const glm::vec3& normal,
            const glm::vec2& texture_coordinates) :
            position(position),
            normal(normal),
            texture_coordinates(texture_coordinates)
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
