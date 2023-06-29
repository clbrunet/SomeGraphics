#include <cstdlib>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/material.h"
#include "assimp/matrix4x4.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/texture.h"
#include "assimp/vector3.h"

#include "SomeGraphics/SceneEntity.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/AssimpToGlm.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

std::optional<std::shared_ptr<SceneEntity>> SceneEntity::load_model(const char* filename)
{
    Assimp::Importer importer;
    const aiScene* ai_scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (ai_scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << std::endl;
        return std::nullopt;
    }
    return std::shared_ptr<SceneEntity>(new SceneEntity(ai_scene->mRootNode,
            aiMatrix4x4(), ai_scene));
}

const glm::mat4& SceneEntity::transform() const
{
    return m_transform;
}

const std::unique_ptr<Mesh>& SceneEntity::mesh() const
{
    return m_mesh;
}

const std::unique_ptr<Texture>& SceneEntity::texture() const
{
    return m_texture;
}

const std::vector<std::shared_ptr<SceneEntity>>& SceneEntity::children() const
{
    return m_children;
}

SceneEntity::SceneEntity(const aiNode* ai_node, const aiMatrix4x4& transform, const aiScene* ai_scene) :
    m_transform(AssimpToGlm::mat4(transform))
{
    assert(ai_node != nullptr && ai_scene != nullptr);
    process_node_meshes(ai_node, ai_scene);
    for (uint i = 0; i < ai_node->mNumChildren; i++) {
        m_children.emplace_back(std::shared_ptr<SceneEntity>(new SceneEntity(ai_node->mChildren[i],
                    transform * ai_node->mChildren[i]->mTransformation, ai_scene)));
    }
}

void SceneEntity::process_node_meshes(const aiNode* ai_node, const aiScene* ai_scene)
{
    assert(ai_node != nullptr && ai_scene != nullptr);
    if (ai_node->mNumMeshes == 0) {
        return;
    }
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    for (uint i = 0; i < ai_node->mNumMeshes; i++) {
        aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
        for(uint i = 0; i < ai_mesh->mNumVertices; i++) {
            vertices.push_back(Vertex(AssimpToGlm::vec3(ai_mesh->mVertices[i]),
                    AssimpToGlm::vec3(ai_mesh->mNormals[i]),
                    AssimpToGlm::vec3(ai_mesh->mTextureCoords[0][i])));
        }
        for (uint i = 0; i < ai_mesh->mNumFaces; i++) {
            aiFace* ai_face = ai_mesh->mFaces + i;
            for (uint j = 0; j < ai_face->mNumIndices; j++) {
                indices.push_back(ai_face->mIndices[j]);
            }
        }
    }
    m_mesh = std::make_unique<Mesh>(vertices, std::initializer_list<VertexAttribute>({
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec2),
    }), indices);
    aiString path;
    ai_scene->mMaterials[ai_scene->mMeshes[ai_node->mMeshes[0]]->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
    const aiTexture* ai_texture = ai_scene->GetEmbeddedTexture(path.C_Str());
    std::optional<std::unique_ptr<Texture>> texture_opt = Texture::from_ai_texture(*ai_texture);
    if (!texture_opt.has_value()) {
        abort();
    }
    m_texture = std::move(texture_opt.value());
}

}
