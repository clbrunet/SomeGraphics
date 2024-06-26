#include <cassert>
#include <iostream>
#include <optional>

#include "SomeGraphics/Rendering/VertexAttribute.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/Skybox.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

std::optional<std::unique_ptr<Skybox>> Skybox::create(const char* right, const char* left,
    const char* top, const char* bottom, const char* front, const char* back)
{
    std::optional<std::shared_ptr<Program>> program_opt
        = ResourcesCache::program("engine/assets/shaders/skybox.vert",
            "engine/assets/shaders/skybox.frag");
    if (!program_opt.has_value()) {
        return std::nullopt;
    }
    std::optional<std::shared_ptr<Texture>> cubemap_opt
        = ResourcesCache::cubemap(right, left, top, bottom, front, back);
    if (!cubemap_opt.has_value()) {
        return std::nullopt;
    }
    std::unique_ptr<VertexArray> vertex_array = std::make_unique<VertexArray>(
        std::span<const Vertex>(std::initializer_list<Vertex>({
            Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
            Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
            Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
            Vertex(glm::vec3(1.0f,  1.0f, -1.0f)),
            Vertex(glm::vec3(-1.0f,  1.0f, 1.0f)),
            Vertex(glm::vec3(-1.0f, -1.0f, 1.0f)),
            Vertex(glm::vec3(1.0f, -1.0f, 1.0f)),
            Vertex(glm::vec3(1.0f,  1.0f, 1.0f)),
        })), Vertex::attributes(),
        std::initializer_list<uint32_t>({
            0, 1, 2, 0, 2, 3,
            4, 0, 3, 4, 3, 7,
            7, 6, 5, 7, 5, 4,
            6, 2, 1, 6, 1, 5,
            3, 2, 6, 3, 6, 7,
            4, 5, 1, 4, 1, 0,
        }));
    return std::unique_ptr<Skybox>(new Skybox(std::move(program_opt.value()),
            std::move(vertex_array), std::move(cubemap_opt.value())));
}

Skybox::~Skybox()
{
    is_instantiated = false;
}

const std::shared_ptr<Program>& Skybox::program() const
{
    return m_program;
}

const std::unique_ptr<VertexArray>& Skybox::vertex_array() const
{
    return m_vertex_array;
}

const std::shared_ptr<Texture>& Skybox::cubemap() const
{
    return m_cubemap;
}

bool Skybox::is_instantiated = false;

Skybox::Skybox(std::shared_ptr<Program>&& program,
    std::unique_ptr<VertexArray>&& vertex_array, std::shared_ptr<Texture>&& cubemap) :
    m_program(std::move(program)),
    m_vertex_array(std::move(vertex_array)),
    m_cubemap(std::move(cubemap))
{
    assert(!is_instantiated && "Multiple instances are not allowed");
    is_instantiated = true;
}

}
