#include <cstdint>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <optional>
#include <memory>

#include "SomeGraphics/Skin.hpp"
#include "SomeGraphics/AssimpHelper.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/Rendering/VertexAttribute.hpp"
#include "SomeGraphics/Entity.hpp"

namespace sg {

std::optional<Skin> Skin::from_ai_node(std::string_view filename,
    const aiNode& ai_node, const aiScene& ai_scene, const std::shared_ptr<Entity>& asset_root)
{
    std::vector<Vertex> vertices;
    std::vector<uint8_t> sub_mesh_vertex_weight_counts;
    std::array<Bone, MAX_BONES_COUNT> bones;
    std::vector<uint32_t> indices;
    std::vector<SubMeshInfo> sub_meshes_info;
    uint32_t indices_offset = 0;
    uint32_t vertices_offset = 0;
    for (uint32_t i = 0; i < ai_node.mNumMeshes; i++) {
        const aiMesh& ai_mesh = *ai_scene.mMeshes[ai_node.mMeshes[i]];
        const aiMaterial& ai_material = *ai_scene.mMaterials[ai_mesh.mMaterialIndex];
        std::optional<std::shared_ptr<Material>> material_opt
            = ResourcesCache::material_from_ai_material(std::string(filename),
                ai_material, ai_scene);
        if (!material_opt.has_value()) {
            return std::nullopt;
        }
        vertices_offset = vertices.size();
        for (uint32_t j = 0; j < ai_mesh.mNumVertices; j++) {
            vertices.emplace_back(Mesh::Vertex(assimp_helper::vec3(ai_mesh.mVertices[j]),
                    assimp_helper::vec3(ai_mesh.mNormals[j]), assimp_helper::vec3(ai_mesh.mTangents[j]),
                    assimp_helper::vec3(ai_mesh.mTextureCoords[0][j])), 0, glm::vec3(0.0));
        }
        sub_mesh_vertex_weight_counts.assign(ai_mesh.mNumVertices, 0);
        if (ai_mesh.mNumBones > MAX_BONES_COUNT) {
            return std::nullopt;
        }
        for (uint32_t j = 0; j < ai_mesh.mNumBones; j++) {
            const aiBone& ai_bone = *ai_mesh.mBones[j];
            for (uint32_t k = 0; k < ai_bone.mNumWeights; k++) {
                aiVertexWeight ai_weight = ai_bone.mWeights[k];
                if (ai_weight.mWeight == 0.0f) {
                    continue;
                }
                Vertex& vertex = vertices[vertices_offset + ai_weight.mVertexId];
                uint8_t& vertex_weight_count = sub_mesh_vertex_weight_counts[ai_weight.mVertexId];
                if (vertex_weight_count >= 4) {
                    return std::nullopt;
                }
                vertex.bone_indices |= j << ((3 - vertex_weight_count) * 8);
                if (vertex_weight_count < 3) {
                    vertex.weights[vertex_weight_count] = ai_weight.mWeight;
                }
                vertex_weight_count++;
            }
            if (bones[j].entity.expired()) {
                bones[j].entity = Entity::search(ai_bone.mName.C_Str(), asset_root).value();
                bones[j].skin_to_bone = assimp_helper::mat4(ai_bone.mOffsetMatrix);
            }
        }
        sub_mesh_vertex_weight_counts.clear();
        indices_offset = indices.size();
        for (uint32_t j = 0; j < ai_mesh.mNumFaces; j++) {
            const aiFace& ai_face = ai_mesh.mFaces[j];
            for (uint32_t k = 0; k < ai_face.mNumIndices; k++) {
                indices.emplace_back(ai_face.mIndices[k]);
            }
        }
        sub_meshes_info.emplace_back(indices.size() - indices_offset,
            (uint8_t*)(indices_offset * sizeof(GLuint)), vertices_offset,
            std::move(material_opt.value()));
    }
    std::unique_ptr<VertexArray> vertex_array
        = std::make_unique<VertexArray>(std::span<const Vertex>(vertices),
            Vertex::attributes(), indices);
    return Skin(std::move(vertex_array), std::move(sub_meshes_info), std::move(bones));
}

const Mesh& Skin::mesh() const
{
    return m_mesh;
}

const std::unique_ptr<VertexArray>& Skin::vertex_array() const
{
    return m_mesh.vertex_array();
}

const std::vector<SubMeshInfo>& Skin::sub_meshes_info() const
{
    return m_mesh.sub_meshes_info();
}
const std::array<Bone, Skin::MAX_BONES_COUNT>& Skin::bones() const
{
    return m_bones;
}

Skin::Skin(std::unique_ptr<VertexArray> vertex_array, std::vector<SubMeshInfo> sub_meshes_info,
    const std::array<Bone, MAX_BONES_COUNT>& bones) :
    m_mesh(std::move(vertex_array), std::move(sub_meshes_info)),
    m_bones(bones)
{
}

}
