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

class Material;

class Entity {
public:
    static std::unique_ptr<Entity> create_scene_root();
    static std::optional<std::shared_ptr<Entity>> load_model(const char* filename);
    Entity(Entity&& other) = default;
    Entity(const Entity& other) = delete;
    Entity& operator=(Entity&& other) = default;
    Entity& operator=(const Entity& other) = delete;
    ~Entity() = default;

    const std::string& name() const;
    const Transform& transform() const;
    Transform& transform();
    const std::shared_ptr<Mesh>& mesh() const;
    const std::shared_ptr<Material>& material() const;
    const std::vector<std::shared_ptr<Entity>>& children() const;

    void add_child(std::shared_ptr<Entity>&& child);
private:
    std::string m_name;
    Transform m_transform;
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
    std::vector<std::shared_ptr<Entity>> m_children;

    // Scene root constructor
    Entity();
    static std::optional<std::shared_ptr<Entity>> from_ai_node(const std::string& filename,
        const aiNode& ai_node, const aiScene& ai_scene);
    Entity(std::string name, Transform transform, std::shared_ptr<Mesh> mesh,
        std::shared_ptr<Material> material, std::vector<std::shared_ptr<Entity>>&& children);
};

}
