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
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>

#include "SomeGraphics/Mesh.hpp"

namespace sg {

class Entity;

struct Bone {
    std::weak_ptr<Entity> entity;
    glm::mat4 skin_to_bone;
};

class Skin {
public:
    constexpr static uint32_t MAX_BONES_COUNT = 256;

    Skin() = delete;
    static std::optional<Skin> from_ai_node(std::string_view filename,
        const aiNode& ai_node, const aiScene& ai_scene, const std::shared_ptr<Entity>& asset_root);
    Skin(Skin&& other) = default;
    Skin(const Skin& other) = delete;
    Skin& operator=(Skin&& other) = default;
    Skin& operator=(const Skin& other) = delete;
    ~Skin() = default;

    const Mesh& mesh() const;
    const std::unique_ptr<VertexArray>& vertex_array() const;
    const std::vector<SubMeshInfo>& sub_meshes_info() const;
    const std::array<Bone, MAX_BONES_COUNT>& bones() const;
private:
    Mesh m_mesh;
    std::array<Bone, MAX_BONES_COUNT> m_bones;

    Skin(std::unique_ptr<VertexArray> vertex_array, std::vector<SubMeshInfo> sub_meshes_info,
        const std::array<Bone, MAX_BONES_COUNT>& bones);

    struct Vertex {
        Mesh::Vertex mesh_vertex;
        uint32_t bone_indices;
        glm::vec3 weights;

        Vertex() = delete;
        Vertex(Mesh::Vertex mesh_vertex, uint32_t bone_indices, glm::vec3 weights) :
            mesh_vertex(std::move(mesh_vertex)),
            bone_indices(std::move(bone_indices)),
            weights(std::move(weights))
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
};

}
