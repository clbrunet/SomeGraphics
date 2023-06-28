#include <iostream>

#include "SomeGraphics/AssimpToGlm.hpp"
#include "assimp/Importer.hpp"
#include "assimp/matrix4x4.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/vector3.h"

#include "SomeGraphics/SceneEntity.hpp"
#include "SomeGraphics/Mesh.hpp"

namespace sg {

std::optional<std::shared_ptr<SceneEntity>> SceneEntity::load_model(const char* filename)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << std::endl;
        return std::nullopt;
    }
    return std::shared_ptr<SceneEntity>(new SceneEntity(scene->mRootNode, aiMatrix4x4(), scene));
}

const glm::mat4& SceneEntity::transform() const
{
    return m_transform;
}

const std::unique_ptr<Mesh>& SceneEntity::mesh() const
{
    return m_mesh;
}

const std::vector<std::shared_ptr<SceneEntity>>& SceneEntity::children() const
{
    return m_children;
}

SceneEntity::SceneEntity(const aiNode* node, const aiMatrix4x4& transform, const aiScene* scene) :
    m_transform(AssimpToGlm::mat4(transform))
{
    assert(node != nullptr && scene != nullptr);
    process_node_meshes(node, scene);
    for (uint i = 0; i < node->mNumChildren; i++) {
        m_children.emplace_back(std::shared_ptr<SceneEntity>(new SceneEntity(node->mChildren[i],
                    transform * node->mChildren[i]->mTransformation, scene)));
    }
}

void SceneEntity::process_node_meshes(const aiNode* node, const aiScene* scene)
{
    assert(node != nullptr && scene != nullptr);
    if (node->mNumMeshes == 0) {
        return;
    }
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    for (uint i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        for(uint i = 0; i < mesh->mNumVertices; i++) {
            vertices.push_back(Vertex(AssimpToGlm::vec3(mesh->mVertices[i]),
                    AssimpToGlm::vec3(mesh->mNormals[i])));
        }
        for (uint i = 0; i < mesh->mNumFaces; i++) {
            aiFace* face = mesh->mFaces + i;
            for (uint j = 0; j < face->mNumIndices; j++) {
                indices.push_back(face->mIndices[j]);
            }
        }
    }
    m_mesh = std::make_unique<Mesh>(vertices, std::initializer_list<VertexAttribute>({
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec3),
    }), indices);
}

}
