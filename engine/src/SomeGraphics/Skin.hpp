#pragma once

#include <array>
#include <cstdint>
#include <glm/ext/matrix_float4x4.hpp>
#include <optional>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <vector>

#include <assimp/scene.h>
#include <entt/entt.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>

#include "SomeGraphics/Mesh.hpp"

namespace sg {

class Node;

struct Bone {
    entt::entity entity;
    glm::mat4 skin_to_bone = glm::mat4(1.0f);
};

class Skin {
public:
    constexpr static uint32_t MAX_BONES_COUNT = 256;

    Skin() = delete;
    static std::optional<Skin> from_ai_node(std::string_view filename,
        const aiNode& ai_node, const aiScene& ai_scene, const Node& asset_root);
    Skin(Skin&& other) = default;
    Skin(const Skin& other) = delete;
    Skin& operator=(Skin&& other) = default;
    Skin& operator=(const Skin& other) = delete;
    ~Skin() = default;

    const Mesh& mesh() const;
    const std::unique_ptr<VertexArray>& vertex_array() const;
    const std::vector<SubMeshInfo>& sub_meshes_info() const;
    const std::vector<Bone>& bones() const;
private:
    Mesh m_mesh;
    std::vector<Bone> m_bones;

    Skin(std::unique_ptr<VertexArray> vertex_array, std::vector<SubMeshInfo> sub_meshes_info,
        std::vector<Bone> bones);
#pragma pack(push, 1)
    struct Vertex {
        Mesh::Vertex mesh_vertex;
        uint32_t bone_indices;
        glm::vec3 weights;

        Vertex() = delete;
        Vertex(Mesh::Vertex p_mesh_vertex, uint32_t p_bone_indices, glm::vec3 p_weights) :
            mesh_vertex(std::move(p_mesh_vertex)),
            bone_indices(std::move(p_bone_indices)),
            weights(std::move(p_weights))
        {
        }
        Vertex(Vertex&& other) = default;
        Vertex(const Vertex& other) = default;
        Vertex& operator=(Vertex&& other) = default;
        Vertex& operator=(const Vertex& other) = default;
        ~Vertex() = default;

        constexpr static std::array<VertexAttribute, 6> attributes()
        {
            std::array<VertexAttribute, 4> mesh_attributes = Mesh::Vertex::attributes();
            return {
                mesh_attributes[0],
                mesh_attributes[1],
                mesh_attributes[2],
                mesh_attributes[3],
                VertexAttribute(VertexAttributeType::Uint),
                VertexAttribute(VertexAttributeType::Vec3),
            };
        }
    };
#pragma pack(pop)
};

}
