#pragma once

#include <optional>
#include <memory>
#include <vector>

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#include "SomeGraphics/Transform.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/Skin.hpp"
#include "SomeGraphics/Light.hpp"

namespace sg {

class Entity {
public:
    static std::unique_ptr<Entity> create_scene_root();
    static std::optional<std::shared_ptr<Entity>> load_model(const char* filename,
        std::shared_ptr<Entity> parent);
    Entity(std::string name, std::weak_ptr<Entity> parent);
    Entity(std::string name, Transform local_transform, std::weak_ptr<Entity> parent);
    Entity(Entity&& other) = default;
    Entity(const Entity& other) = delete;
    Entity& operator=(Entity&& other) = default;
    Entity& operator=(const Entity& other) = delete;
    ~Entity() = default;

    const std::string& name() const;

    const glm::vec3& local_position() const;
    const glm::vec3& local_rotation() const;
    const glm::vec3& local_scale() const;

    void set_local_position(glm::vec3 local_position);
    void set_local_rotation(glm::vec3 local_rotation);
    void set_local_scale(glm::vec3 local_scale);

    const glm::mat4& model_matrix() const;

    // TODO: remove these by using an ECS
    const std::shared_ptr<Mesh>& mesh() const;
    const std::shared_ptr<Skin>& skin() const;
    std::unique_ptr<Light>& light();
    const std::vector<std::shared_ptr<Entity>>& children() const;

    void set_light(std::unique_ptr<Light>&& light);

    void add_child(std::shared_ptr<Entity> child);

    static std::optional<std::shared_ptr<Entity>> search(std::string_view name,
        const std::shared_ptr<Entity>& entity);
private:
    std::string m_name;
    Transform m_local_transform;
    mutable glm::mat4 m_model_matrix = glm::mat4(1.0f);
    mutable bool m_is_model_matrix_dirty = true;
    std::weak_ptr<Entity> m_parent;
    std::vector<std::shared_ptr<Entity>> m_children;
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Skin> m_skin;
    std::unique_ptr<Light> m_light;

    struct TwoPassSkinRefs {
        const aiNode& ai_node;
        std::shared_ptr<Entity> entity;

        TwoPassSkinRefs(const aiNode& ai_node, std::shared_ptr<Entity> entity) :
            ai_node(ai_node), entity(entity)
        {
        }
    };

    // Scene root constructor
    Entity();
    static std::optional<std::shared_ptr<Entity>> load_model_first_pass(std::string_view filename,
        const aiNode& ai_node, const aiScene& ai_scene, std::shared_ptr<Entity> parent,
        std::vector<TwoPassSkinRefs>& two_pass_skin_refs_vec);
    Entity(std::string name, Transform local_transform, std::weak_ptr<Entity> parent,
        std::shared_ptr<Mesh> mesh);
    Entity(std::string name, Transform local_transform, std::weak_ptr<Entity> parent,
        std::shared_ptr<Skin> skin);

    void set_model_matrix_dirty();
    static bool is_ai_node_skin(const aiNode& ai_node, const aiScene& ai_scene);
};

}
