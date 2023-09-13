#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

#include "SomeGraphics/Rendering/Material.hpp"
#include "assimp/Importer.hpp"
#include "assimp/StringUtils.h"
#include "assimp/material.h"
#include "assimp/matrix4x4.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/texture.h"
#include "assimp/types.h"
#include "assimp/vector3.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/gtx/string_cast.hpp"

#include "SomeGraphics/Entity.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/AssimpToGlm.hpp"

namespace sg {

std::unique_ptr<Entity> Entity::create_scene_root()
{
    return std::unique_ptr<Entity>(new Entity());
}

std::optional<std::shared_ptr<Entity>> Entity::load_model(const char* filename)
{
    Assimp::Importer importer;
    const aiScene* ai_scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (ai_scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << std::endl;
        return std::nullopt;
    }
    return from_ai_node(filename, *ai_scene->mRootNode, *ai_scene);
}

const std::string& Entity::name() const
{
    return m_name;
}

const Transform& Entity::transform() const
{
    return m_transform;
}

Transform& Entity::transform()
{
    return m_transform;
}

const std::shared_ptr<Mesh>& Entity::mesh() const
{
    return m_mesh;
}

const std::shared_ptr<Material>& Entity::material() const
{
    return m_material;
}

const std::vector<std::shared_ptr<Entity>>& Entity::children() const
{
    return m_children;
}

void Entity::add_child(std::shared_ptr<Entity>&& child)
{
    m_children.emplace_back(std::move(child));
}

Entity::Entity() :
    m_name("SceneRoot")
{
}

std::optional<std::shared_ptr<Entity>> Entity::from_ai_node(
    const std::string& filename, const aiNode& ai_node, const aiScene& ai_scene)
{
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
    if (ai_node.mNumMeshes > 0) {
        const aiMaterial& ai_material = *ai_scene.mMaterials[
            ai_scene.mMeshes[ai_node.mMeshes[0]]->mMaterialIndex];
        std::optional<std::shared_ptr<Material>> material_opt
            = ResourcesCache::material_from_ai_material(filename, ai_material, ai_scene);
        if (!material_opt.has_value()) {
            return std::nullopt;
        }
        mesh = ResourcesCache::mesh_from_ai_node(filename, ai_node, ai_scene);
        material = material_opt.value();
    }
    std::vector<std::shared_ptr<Entity>> children;
    for (uint32_t i = 0; i < ai_node.mNumChildren; i++) {
        std::optional<std::shared_ptr<Entity>> child_opt
            = from_ai_node(filename, *ai_node.mChildren[i], ai_scene);
        if (!child_opt.has_value()) {
            return std::nullopt;
        }
        children.emplace_back(std::move(child_opt.value()));
    }
    std::string name = ai_node.mName.C_Str();
    Transform transform = Transform(AssimpToGlm::mat4(ai_node.mTransformation));
    return std::shared_ptr<Entity>(new Entity(std::move(name), std::move(transform),
            std::move(mesh), std::move(material), std::move(children)));
}

Entity::Entity(std::string name, Transform transform, std::shared_ptr<Mesh> mesh,
    std::shared_ptr<Material> material, std::vector<std::shared_ptr<Entity>>&& children) :
    m_name(std::move(name)),
    m_transform(std::move(transform)),
    m_mesh(std::move(mesh)),
    m_material(std::move(material)),
    m_children(std::move(children))
{
}

}
