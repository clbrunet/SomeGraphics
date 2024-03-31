#pragma once

#include <optional>
#include <memory>
#include <array>

#include <glm/ext/vector_float3.hpp>

#include "SomeGraphics/Rendering/VertexAttribute.hpp"

namespace sg {

class Program;
class VertexArray;
class Texture;

class Skybox {
public:
    Skybox() = delete;
    static std::optional<std::unique_ptr<Skybox>> create(const char* right, const char* left,
        const char* top, const char* bottom, const char* front, const char* back);
    Skybox(Skybox&& other) = default;
    Skybox(const Skybox& other) = delete;
    Skybox& operator=(Skybox&& other) = delete;
    Skybox& operator=(const Skybox& other) = delete;
    ~Skybox();

    const std::shared_ptr<Program>& program() const;
    const std::unique_ptr<VertexArray>& vertex_array() const;
    const std::shared_ptr<Texture>& cubemap() const;
private:
    std::shared_ptr<Program> m_program;
    std::unique_ptr<VertexArray> m_vertex_array;
    std::shared_ptr<Texture> m_cubemap;
    static bool is_instantiated;

    Skybox(std::shared_ptr<Program>&& program,
        std::unique_ptr<VertexArray>&& vertex_array, std::shared_ptr<Texture>&& cubemap);
#pragma pack(push, 1)
    struct Vertex {
        glm::vec3 position;

        Vertex() = delete;
        Vertex(glm::vec3 p_position) :
            position(std::move(p_position))
        {
        }
        Vertex(Vertex&& other) = default;
        Vertex(const Vertex& other) = default;
        Vertex& operator=(Vertex&& other) = default;
        Vertex& operator=(const Vertex& other) = default;
        ~Vertex() = default;

        constexpr static std::array<VertexAttribute, 1> attributes()
        {
            return {
                VertexAttribute(VertexAttributeType::Vec3),
            };
        }
    };
#pragma pack(pop)
};

}
