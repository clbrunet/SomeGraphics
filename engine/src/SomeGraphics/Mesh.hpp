#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <iostream>
#include <memory>
#include <sys/types.h>
#include <vector>

#include <assimp/scene.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtx/string_cast.hpp>

#include "SomeGraphics/Rendering/VertexArray.hpp"

namespace sg {

class Material;

class SubMeshInfo {
public:
    SubMeshInfo() = delete;
    SubMeshInfo(uint32_t indices_count, uint8_t* index_buffer_offset,
        uint32_t vergices_offset, std::shared_ptr<Material> material);
    SubMeshInfo(SubMeshInfo&& other) = default;
    SubMeshInfo(const SubMeshInfo& other) = delete;
    SubMeshInfo& operator=(SubMeshInfo&& other) = default;
    SubMeshInfo& operator=(const SubMeshInfo& other) = delete;
    ~SubMeshInfo() = default;

    uint32_t indices_count() const;
    uint8_t* index_buffer_offset() const;
    uint32_t vertices_offset() const;
    const std::shared_ptr<Material>& material() const;
private:
    uint32_t m_indices_count = 0;
    uint8_t* m_index_buffer_offset = 0;
    uint32_t m_vertices_offset = 0;
    std::shared_ptr<Material> m_material;
};

class Mesh {
public:
    static std::optional<Mesh> from_ai_node(std::string_view filename,
        const aiNode& ai_node, const aiScene& ai_scene);
    Mesh() = delete;
    Mesh(std::unique_ptr<VertexArray> vertex_array, std::shared_ptr<Material> material);
    Mesh(std::unique_ptr<VertexArray> vertex_array, std::vector<SubMeshInfo> sub_meshes_info);
    Mesh(Mesh&& other) = default;
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) = default;
    Mesh& operator=(const Mesh& other) = delete;
    ~Mesh() = default;

    const std::unique_ptr<VertexArray>& vertex_array() const;
    const std::vector<SubMeshInfo>& sub_meshes_info() const;
#pragma pack(push, 1)
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 texture_coordinates;

        Vertex() = delete;
        Vertex(glm::vec3 p_position, glm::vec3 p_normal, glm::vec3 p_tangent,
            glm::vec2 p_texture_coordinates) :
            position(std::move(p_position)),
            normal(std::move(p_normal)),
            tangent(std::move(p_tangent)),
            texture_coordinates(std::move(p_texture_coordinates))
        {
        }
        Vertex(Vertex&& other) = default;
        Vertex(const Vertex& other) = default;
        Vertex& operator=(Vertex&& other) = default;
        Vertex& operator=(const Vertex& other) = default;
        ~Vertex() = default;

        constexpr static std::array<VertexAttribute, 4> attributes()
        {
            return {
                VertexAttribute(VertexAttributeType::Vec3),
                VertexAttribute(VertexAttributeType::Vec3),
                VertexAttribute(VertexAttributeType::Vec3),
                VertexAttribute(VertexAttributeType::Vec2),
            };
        }
    };
#pragma pack(pop)
private:
    std::unique_ptr<VertexArray> m_vertex_array;
    std::vector<SubMeshInfo> m_sub_meshes_info;
};

}
