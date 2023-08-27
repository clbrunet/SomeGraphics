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

#include "SomeGraphics/SceneEntity.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/AssimpToGlm.hpp"

namespace sg {

std::unique_ptr<SceneEntity> SceneEntity::create_scene_root()
{
    return std::unique_ptr<SceneEntity>(new SceneEntity());
}

std::optional<std::shared_ptr<SceneEntity>> SceneEntity::load_model(const char* filename)
{
    Assimp::Importer importer;
    const aiScene* ai_scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (ai_scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << std::endl;
        return std::nullopt;
    }
    return std::shared_ptr<SceneEntity>(new SceneEntity(filename,
            *ai_scene->mRootNode, *ai_scene));
}

const std::string& SceneEntity::name() const
{
    return m_name;
}

const Transform& SceneEntity::transform() const
{
    return m_transform;
}

Transform& SceneEntity::transform()
{
    return m_transform;
}

const std::shared_ptr<Mesh>& SceneEntity::mesh() const
{
    return m_mesh;
}

const std::shared_ptr<Material>& SceneEntity::material() const
{
    return m_material;
}

const std::vector<std::shared_ptr<SceneEntity>>& SceneEntity::children() const
{
    return m_children;
}

void SceneEntity::add_child(std::shared_ptr<SceneEntity>&& child)
{
    m_children.emplace_back(std::move(child));
}

SceneEntity::SceneEntity() :
    m_name("SceneRoot")
{
}

SceneEntity::SceneEntity(const std::string& filename,
    const aiNode& ai_node, const aiScene& ai_scene) :
    m_name(ai_node.mName.C_Str()),
    m_transform(AssimpToGlm::mat4(ai_node.mTransformation))
{
    process_node(filename, ai_node, ai_scene);
    for (uint32_t i = 0; i < ai_node.mNumChildren; i++) {
        const aiNode& child = *ai_node.mChildren[i];
        m_children.emplace_back(std::shared_ptr<SceneEntity>(new SceneEntity(filename,
                    child, ai_scene)));
    }
}

void SceneEntity::process_node(const std::string& filename,
    const aiNode& ai_node, const aiScene& ai_scene)
{
    if (ai_node.mNumMeshes == 0) {
        return;
    }
    m_mesh = ResourcesCache::mesh_from_ai_node(filename, ai_node, ai_scene);
    const aiMaterial& ai_material = *ai_scene.mMaterials[
        ai_scene.mMeshes[ai_node.mMeshes[0]]->mMaterialIndex];
    std::optional<std::shared_ptr<Material>> material_opt
        = ResourcesCache::material_from_ai_material(filename, ai_material, ai_scene);
    if (!material_opt.has_value()) {
        abort();
    }
    m_material = std::move(material_opt.value());
}

}
