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
#include "SomeGraphics/AssimpToGlm.hpp"
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
    const aiScene* ai_scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (ai_scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << std::endl;
        return std::nullopt;
    }
    return from_ai_node(filename, *ai_scene->mRootNode, *ai_scene, std::move(parent));
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

const std::shared_ptr<Material>& Entity::material() const
{
    return m_material;
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

Entity::Entity() :
    m_name("SceneRoot"),
    m_is_model_matrix_dirty(false)
{
}

std::optional<std::shared_ptr<Entity>> Entity::from_ai_node(const std::string& filename,
    const aiNode& ai_node, const aiScene& ai_scene, std::shared_ptr<Entity> parent)
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
    std::string name = ai_node.mName.C_Str();
    Transform transform = Transform(AssimpToGlm::mat4(ai_node.mTransformation));
    std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(std::move(name),
            std::move(transform), parent, std::move(mesh), std::move(material)));
    for (uint32_t i = 0; i < ai_node.mNumChildren; i++) {
        if (!from_ai_node(filename, *ai_node.mChildren[i], ai_scene, entity).has_value()) {
            return std::nullopt;
        }
    }
    parent->add_child(entity);
    return entity;
}

Entity::Entity(std::string name, Transform local_transform, std::weak_ptr<Entity> parent,
    std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material) :
    m_name(std::move(name)),
    m_local_transform(std::move(local_transform)),
    m_parent(std::move(parent)),
    m_mesh(std::move(mesh)),
    m_material(std::move(material))
{
}

void Entity::set_model_matrix_dirty()
{
    m_is_model_matrix_dirty = true;
    for (std::shared_ptr<Entity>& entity : m_children) {
        entity->set_model_matrix_dirty();
    }
}

}
