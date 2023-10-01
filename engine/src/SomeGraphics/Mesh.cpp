#include <cstdint>
#include <iostream>
#include <optional>
#include <memory>

#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/AssimpToGlm.hpp"
#include "SomeGraphics/ResourcesCache.hpp"

namespace sg {

SubMeshInfo::SubMeshInfo(uint32_t indices_count, uint8_t* index_buffer_offset,
    uint32_t vertices_offset, std::shared_ptr<Material> material) :
    m_indices_count(indices_count),
    m_index_buffer_offset(index_buffer_offset),
    m_vertices_offset(vertices_offset),
    m_material(std::move(material))
{
}

uint32_t SubMeshInfo::indices_count() const
{
    return m_indices_count;
}

uint8_t* SubMeshInfo::index_buffer_offset() const
{
    return m_index_buffer_offset;
}

uint32_t SubMeshInfo::vertices_offset() const
{
    return m_vertices_offset;
}

const std::shared_ptr<Material> SubMeshInfo::material() const
{
    return m_material;
}

std::optional<std::unique_ptr<Mesh>> Mesh::from_ai_node(const std::string& filename,
    const aiNode& ai_node, const aiScene& ai_scene)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<SubMeshInfo> sub_meshes_info;
    uint32_t indices_offset = 0;
    uint32_t vertices_offset = 0;
    for (uint32_t i = 0; i < ai_node.mNumMeshes; i++) {
        const aiMesh& ai_mesh = *ai_scene.mMeshes[ai_node.mMeshes[i]];
        const aiMaterial& ai_material = *ai_scene.mMaterials[ai_mesh.mMaterialIndex];
        std::optional<std::shared_ptr<Material>> material_opt
            = ResourcesCache::material_from_ai_material(filename, ai_material, ai_scene);
        if (!material_opt.has_value()) {
            return std::nullopt;
        }
        vertices_offset = vertices.size();
        for(uint32_t j = 0; j < ai_mesh.mNumVertices; j++) {
            vertices.push_back(Vertex(AssimpToGlm::vec3(ai_mesh.mVertices[j]),
                    AssimpToGlm::vec3(ai_mesh.mNormals[j]),
                    AssimpToGlm::vec3(ai_mesh.mTextureCoords[0][j])));
        }
        indices_offset = indices.size();
        for (uint32_t j = 0; j < ai_mesh.mNumFaces; j++) {
            const aiFace& ai_face = ai_mesh.mFaces[j];
            for (uint32_t k = 0; k < ai_face.mNumIndices; k++) {
                indices.push_back(ai_face.mIndices[k]);
            }
        }
        sub_meshes_info.emplace_back(indices.size() - indices_offset,
            (uint8_t*)(indices_offset * sizeof(GLuint)), vertices_offset,
            std::move(material_opt.value()));
    }
    std::unique_ptr<VertexArray> vertex_array = std::make_unique<VertexArray>(vertices,
        std::initializer_list<VertexAttribute>({
            VertexAttribute(VertexAttributeType::Vec3),
            VertexAttribute(VertexAttributeType::Vec3),
            VertexAttribute(VertexAttributeType::Vec2),
        }), indices);
    return std::make_unique<Mesh>(std::move(vertex_array), std::move(sub_meshes_info));
}

Mesh::Mesh(std::unique_ptr<VertexArray> vertex_array, std::shared_ptr<Material> material) :
    m_vertex_array(std::move(vertex_array))
{
    m_sub_meshes_info.emplace_back(m_vertex_array->index_buffer()->count(),
        (uint8_t*)0, 0, std::move(material));
}

Mesh::Mesh(std::unique_ptr<VertexArray> vertex_array, std::vector<SubMeshInfo> sub_meshes_info) :
    m_vertex_array(std::move(vertex_array)),
    m_sub_meshes_info(std::move(sub_meshes_info))
{
}

const std::unique_ptr<VertexArray>& Mesh::vertex_array() const
{
    return m_vertex_array;
}

const std::vector<SubMeshInfo>& Mesh::sub_meshes_info() const
{
    return m_sub_meshes_info;
}

}
