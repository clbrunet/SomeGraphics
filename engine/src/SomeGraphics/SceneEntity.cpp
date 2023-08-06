#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

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

#include "SomeGraphics/SceneEntity.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/AssimpToGlm.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/gtx/string_cast.hpp"

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

const std::shared_ptr<Texture>& SceneEntity::albedo() const
{
    return m_albedo;
}

const std::shared_ptr<Texture>& SceneEntity::roughness() const
{
    return m_roughness;
}

const std::shared_ptr<Texture>& SceneEntity::metalness() const
{
    return m_metalness;
}

const glm::vec4& SceneEntity::color() const
{
    return m_color;
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
    for (uint i = 0; i < ai_node.mNumChildren; i++) {
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
    aiColor3D ai_color3;
    ai_material.Get(AI_MATKEY_BASE_COLOR, ai_color3);
    m_color = glm::vec4(AssimpToGlm::vec3(ai_color3), 1.0f);
    struct TextureInfo {
        aiTextureType type;
        std::shared_ptr<Texture>& texture;
        ColorSpace color_space;
    };
    std::array<TextureInfo, 3> textures_info = {
        TextureInfo { aiTextureType_BASE_COLOR, m_albedo, ColorSpace::Srgb },
        TextureInfo { aiTextureType_DIFFUSE_ROUGHNESS, m_roughness, ColorSpace::Linear },
        TextureInfo { aiTextureType_METALNESS, m_metalness, ColorSpace::Linear },
    };
    for (const TextureInfo& texture_info : textures_info) {
        if (ai_material.GetTextureCount(texture_info.type) == 0) {
            texture_info.texture = ResourcesCache::white_1px_texture();
        } else {
            aiString path;
            ai_material.GetTexture(texture_info.type, 0, &path);
            const aiTexture* ai_texture = ai_scene.GetEmbeddedTexture(path.C_Str());
            std::optional<std::shared_ptr<Texture>> texture_opt
                = ResourcesCache::texture_from_ai_texture(filename, *ai_texture,
                    texture_info.color_space);
            if (!texture_opt.has_value()) {
                abort();
            }
            texture_info.texture = std::move(texture_opt.value());
        }
    }
}

}
