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
    const std::shared_ptr<Mesh>& mesh() const;
    const std::shared_ptr<Material>& material() const;
    const std::vector<std::shared_ptr<SceneEntity>>& children() const;

    void add_child(std::shared_ptr<SceneEntity>&& child);
private:
    std::string m_name;
    Transform m_transform;
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
    std::vector<std::shared_ptr<SceneEntity>> m_children;

    // Scene root constructor
    SceneEntity();
    static std::optional<std::shared_ptr<SceneEntity>> from_ai_node(const std::string& filename,
        const aiNode& ai_node, const aiScene& ai_scene);
    SceneEntity(std::string name, Transform transform, std::shared_ptr<Mesh> mesh,
        std::shared_ptr<Material> material, std::vector<std::shared_ptr<SceneEntity>>&& children);
};

}
