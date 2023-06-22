#include <iostream>
#include <optional>

#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/vector3.h"
#include "glm/gtx/string_cast.hpp"

#include "SomeGraphics/Model.hpp"
#include "SomeGraphics/Mesh.hpp"

namespace sg {

std::optional<std::unique_ptr<Model>> Model::load(const char* filename)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
    if (scene == nullptr) {
        std::cerr << "Model loading error : " << importer.GetErrorString() << std::endl;
        return std::nullopt;
    }
    return std::unique_ptr<Model>(new Model(scene));
}

Model::Model(std::vector<Mesh>&& meshes) :
    m_meshes(std::move(meshes))
{
}

Model::~Model()
{
}

const std::vector<Mesh>& Model::meshes() const
{
    return m_meshes;
}

Model::Model(const aiScene* scene)
{
    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    for (uint i = 0; i < node->mNumMeshes; i++) {
        process_mesh(scene->mMeshes[node->mMeshes[i]]);
    }
    for (uint i = 0; i < node->mNumChildren; i++) {
        process_node(node->mChildren[i], scene);
    }
}

void Model::process_mesh(aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    for(uint i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D* position = mesh->mVertices + i;
        aiVector3D* normal = mesh->mNormals + i;
        vertices.push_back(Vertex(glm::vec3(position->x, position->y, position->z),
                glm::vec3(normal->x, normal->y, normal->z)));
    }
    std::vector<uint> indices;
    for (uint i = 0; i < mesh->mNumFaces; i++) {
        aiFace* face = mesh->mFaces + i;
        for (uint j = 0; j < face->mNumIndices; j++) {
            indices.push_back(face->mIndices[j]);
        }
    }
    m_meshes.emplace_back(vertices, std::initializer_list<VertexAttribute>({
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec3),
    }), indices);

}

}
