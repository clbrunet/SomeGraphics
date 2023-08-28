#include <iostream>
#include <memory>

#include "SomeGraphics/AssimpToGlm.hpp"
#include "SomeGraphics/Mesh.hpp"

namespace sg {

Mesh::Mesh(const aiNode& ai_node, const aiScene& ai_scene)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    for (uint32_t i = 0; i < ai_node.mNumMeshes; i++) {
        const aiMesh& ai_mesh = *ai_scene.mMeshes[ai_node.mMeshes[i]];
        for(uint32_t j = 0; j < ai_mesh.mNumVertices; j++) {
            vertices.push_back(Vertex(AssimpToGlm::vec3(ai_mesh.mVertices[j]),
                    AssimpToGlm::vec3(ai_mesh.mNormals[j]),
                    AssimpToGlm::vec3(ai_mesh.mTextureCoords[0][j])));
        }
        for (uint32_t j = 0; j < ai_mesh.mNumFaces; j++) {
            const aiFace& ai_face = ai_mesh.mFaces[j];
            for (uint32_t k = 0; k < ai_face.mNumIndices; k++) {
                indices.push_back(ai_face.mIndices[k]);
            }
        }
    }
    m_vertex_array = std::make_unique<VertexArray>(vertices, std::initializer_list<VertexAttribute>({
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec2),
    }), indices);
}

const std::unique_ptr<VertexArray>& Mesh::vertex_array() const
{
    return m_vertex_array;
}

}
