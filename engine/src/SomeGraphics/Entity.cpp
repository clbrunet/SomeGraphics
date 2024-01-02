#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <optional>
#include <utility>

#include <assimp/Importer.hpp>
#include <assimp/StringUtils.h>
#include <assimp/material.h>
#include <assimp/matrix4x4.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/texture.h>
#include <assimp/types.h>
#include <assimp/vector3.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtx/string_cast.hpp>

#include "SomeGraphics/Entity.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/AssimpHelper.hpp"
#include "SomeGraphics/Rendering/Material.hpp"

namespace sg {

std::unique_ptr<Entity> Entity::create_scene_root()
{
    return std::unique_ptr<Entity>(new Entity());
}

std::optional<std::shared_ptr<Entity>> Entity::load_model(const char* filename,
    std::shared_ptr<Entity> parent)
{
    Assimp::Importer importer;
    const aiScene* ai_scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs
        | aiProcess_GenNormals | aiProcess_CalcTangentSpace);
    if (ai_scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << '\n';
        return std::nullopt;
    }
    std::vector<TwoPassSkinRefs> two_pass_skin_refs_vec;
    std::optional<std::shared_ptr<Entity>> entity_opt = load_model_first_pass(filename,
        *ai_scene->mRootNode, *ai_scene, std::move(parent), two_pass_skin_refs_vec);
    if (!entity_opt.has_value()) {
        return std::nullopt;
    }
    for (const TwoPassSkinRefs& two_pass_skin_refs : two_pass_skin_refs_vec) {
        std::optional<std::shared_ptr<Skin>> skin_opt
            = ResourcesCache::skin_from_ai_node(filename, two_pass_skin_refs.ai_node,
                *ai_scene, entity_opt.value());
        if (!skin_opt.has_value()) {
            return std::nullopt;
        }
        two_pass_skin_refs.entity->m_skin = std::move(skin_opt.value());
    }
    return entity_opt;
}

Entity::Entity(std::string name, std::weak_ptr<Entity> parent) :
    m_name(std::move(name)),
    m_parent(std::move(parent))
{
}

Entity::Entity(std::string name, Transform local_transform, std::weak_ptr<Entity> parent) :
    m_name(std::move(name)),
    m_local_transform(std::move(local_transform)),
    m_parent(std::move(parent))
{
}

const std::string& Entity::name() const
{
    return m_name;
}

const glm::vec3& Entity::local_position() const
{
    return m_local_transform.position;
}

const glm::vec3& Entity::local_rotation() const
{
    return m_local_transform.rotation;
}

const glm::vec3& Entity::local_scale() const
{
    return m_local_transform.scale;
}

void Entity::set_local_position(glm::vec3 local_position)
{
    m_local_transform.position = local_position;
    set_model_matrix_dirty();
}

void Entity::set_local_rotation(glm::vec3 local_rotation)
{
    m_local_transform.rotation = local_rotation;
    set_model_matrix_dirty();
}

void Entity::set_local_scale(glm::vec3 local_scale)
{
    m_local_transform.scale = local_scale;
    set_model_matrix_dirty();
}

const glm::mat4& Entity::model_matrix() const
{
    if (m_is_model_matrix_dirty) {
        m_model_matrix = m_parent.lock()->model_matrix()
            * m_local_transform.compute_matrix();
        m_is_model_matrix_dirty = false;
    }
    return m_model_matrix;
}

const std::shared_ptr<Mesh>& Entity::mesh() const
{
    return m_mesh;
}

const std::shared_ptr<Skin>& Entity::skin() const
{
    return m_skin;
}

std::unique_ptr<Light>& Entity::light()
{
    return m_light;
}

const std::vector<std::shared_ptr<Entity>>& Entity::children() const
{
    return m_children;
}

void Entity::set_light(std::unique_ptr<Light>&& light)
{
    m_light = std::move(light);
}

void Entity::add_child(std::shared_ptr<Entity> child)
{
    m_children.emplace_back(std::move(child));
}

std::optional<std::shared_ptr<Entity>> Entity::search(std::string_view name,
    const std::shared_ptr<Entity>& entity)
{
    if (entity->m_name == name) {
        return entity;
    }
    for (const std::shared_ptr<Entity>& child : entity->m_children) {
        auto entity_opt = search(name, child);
        if (entity_opt.has_value()) {
            return entity_opt;
        }
    }
    return std::nullopt;
}

Entity::Entity() :
    m_name("SceneRoot"),
    m_is_model_matrix_dirty(false)
{
}

std::optional<std::shared_ptr<Entity>> Entity::load_model_first_pass(std::string_view filename,
    const aiNode& ai_node, const aiScene& ai_scene, std::shared_ptr<Entity> parent,
    std::vector<TwoPassSkinRefs>& two_pass_skin_refs_vec)
{
    bool is_skin = false;
    std::shared_ptr<Mesh> mesh;
    if (ai_node.mNumMeshes > 0) {
        if (is_ai_node_skin(ai_node, ai_scene)) {
            is_skin = true;
        }
        else {
            std::optional<std::shared_ptr<Mesh>> mesh_opt
                = ResourcesCache::mesh_from_ai_node(std::string(filename), ai_node, ai_scene);
            if (!mesh_opt.has_value()) {
                return std::nullopt;
            }
            mesh = std::move(mesh_opt.value());
        }
    }
    std::string name = ai_node.mName.C_Str();
    Transform transform = Transform(assimp_helper::mat4(ai_node.mTransformation));
    std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(std::move(name),
            std::move(transform), parent, std::move(mesh)));
    if (is_skin) {
        two_pass_skin_refs_vec.emplace_back(ai_node, entity);
    }
    for (uint32_t i = 0; i < ai_node.mNumChildren; i++) {
        if (!load_model_first_pass(filename, *ai_node.mChildren[i],
                ai_scene, entity, two_pass_skin_refs_vec).has_value()) {
            return std::nullopt;
        }
    }
    parent->add_child(entity);
    return entity;
}

Entity::Entity(std::string name, Transform local_transform, std::weak_ptr<Entity> parent,
    std::shared_ptr<Mesh> mesh) :
    m_name(std::move(name)),
    m_local_transform(std::move(local_transform)),
    m_parent(std::move(parent)),
    m_mesh(std::move(mesh))
{
}

Entity::Entity(std::string name, Transform local_transform, std::weak_ptr<Entity> parent,
    std::shared_ptr<Skin> skin) :
    m_name(std::move(name)),
    m_local_transform(std::move(local_transform)),
    m_parent(std::move(parent)),
    m_skin(std::move(skin))
{
}

void Entity::set_model_matrix_dirty()
{
    m_is_model_matrix_dirty = true;
    for (std::shared_ptr<Entity>& entity : m_children) {
        entity->set_model_matrix_dirty();
    }
}

bool Entity::is_ai_node_skin(const aiNode& ai_node, const aiScene& ai_scene)
{
    for (uint32_t i = 0; i < ai_node.mNumMeshes; i++) {
        if (ai_scene.mMeshes[ai_node.mMeshes[i]]->HasBones()) {
            return true;
        }
    }
    return false;
}

}
