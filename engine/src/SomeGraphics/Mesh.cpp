#include <iostream>
#include <memory>

#include "SomeGraphics/AssimpToGlm.hpp"
#include "SomeGraphics/Mesh.hpp"

namespace sg {

Mesh::Mesh(const aiNode& ai_node, const aiScene& ai_scene)
{
    std::vector<Vertex> vertices;
    std::vector<uint> indices;
    for (uint i = 0; i < ai_node.mNumMeshes; i++) {
        const aiMesh& ai_mesh = *ai_scene.mMeshes[ai_node.mMeshes[i]];
        for(uint i = 0; i < ai_mesh.mNumVertices; i++) {
            vertices.push_back(Vertex(AssimpToGlm::vec3(ai_mesh.mVertices[i]),
                    AssimpToGlm::vec3(ai_mesh.mNormals[i]),
                    AssimpToGlm::vec3(ai_mesh.mTextureCoords[0][i])));
        }
        for (uint i = 0; i < ai_mesh.mNumFaces; i++) {
            const aiFace& ai_face = ai_mesh.mFaces[i];
            for (uint j = 0; j < ai_face.mNumIndices; j++) {
                indices.push_back(ai_face.mIndices[j]);
            }
        }
    }
    m_vertex_array = std::make_unique<VertexArray>(vertices, std::initializer_list<VertexAttribute>({
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec3),
        VertexAttribute(VertexAttributeType::Vec2),
    }), indices);
}

const VertexArray& Mesh::vertex_array() const
{
    return *m_vertex_array;
}

}
