#pragma once

#include <optional>
#include <memory>
#include <array>

#include <glm/ext/vector_float2.hpp>

#include "SomeGraphics/Rendering/VertexAttribute.hpp"

namespace sg {

class Program;
class VertexArray;
class Texture;

class PostProcess {
public:
    PostProcess();
    PostProcess(PostProcess&& other) = default;
    PostProcess(const PostProcess& other) = delete;
    PostProcess& operator=(PostProcess&& other) = default;
    PostProcess& operator=(const PostProcess& other) = delete;
    ~PostProcess() = default;

    const std::shared_ptr<Program>& program() const;
    const std::unique_ptr<VertexArray>& vertex_array() const;
private:
    std::shared_ptr<Program> m_program;
    std::unique_ptr<VertexArray> m_vertex_array;
    #pragma pack(push, 1)
    struct Vertex {
        glm::vec2 position;
        glm::vec2 texture_coordinates;

        Vertex() = delete;
        Vertex(glm::vec2 p_position, glm::vec2 p_texture_coordinates) :
            position(std::move(p_position)),
            texture_coordinates(std::move(p_texture_coordinates))
        {
        }
        Vertex(Vertex&& other) = default;
        Vertex(const Vertex& other) = default;
        Vertex& operator=(Vertex&& other) = default;
        Vertex& operator=(const Vertex& other) = default;
        ~Vertex() = default;

        constexpr static std::array<VertexAttribute, 2> attributes()
        {
            return {
                VertexAttribute(VertexAttributeType::Vec2),
                VertexAttribute(VertexAttributeType::Vec2),
            };
        }
    };
#pragma pack(pop)
};

}
