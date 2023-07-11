#pragma once

#include <optional>
#include <memory>
#include <vector>

#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "glm/ext/vector_float3.hpp"

#include "SomeGraphics/Transform.hpp"
#include "SomeGraphics/Mesh.hpp"

namespace sg {

class Texture;

class SceneEntity {
public:
    static std::unique_ptr<SceneEntity> create_scene_root();
    static std::optional<std::shared_ptr<SceneEntity>> load_model(const char* filename);
    SceneEntity(SceneEntity&& other) = default;
    SceneEntity(const SceneEntity& other) = delete;
    SceneEntity& operator=(SceneEntity&& other) = default;
    SceneEntity& operator=(const SceneEntity& other) = delete;
    ~SceneEntity() = default;

    const std::string& name() const;
    const Transform& transform() const;
    Transform& transform();
    const std::unique_ptr<Mesh>& mesh() const;
    const std::shared_ptr<Texture>& texture() const;
    const glm::vec3& color() const;
    const std::vector<std::shared_ptr<SceneEntity>>& children() const;

    void add_child(std::shared_ptr<SceneEntity>&& child);
private:
    std::string m_name;
    Transform m_transform;
    std::unique_ptr<Mesh> m_mesh;
    std::shared_ptr<Texture> m_texture;
    glm::vec3 m_color;
    std::vector<std::shared_ptr<SceneEntity>> m_children;

    // Scene root constructor
    SceneEntity();
    SceneEntity(const std::string& filename, const aiNode* node, const aiScene* scene);

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
