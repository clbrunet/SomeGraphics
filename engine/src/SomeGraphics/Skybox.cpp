#include <cassert>
#include <iostream>
#include <optional>

#include "SomeGraphics/Rendering/VertexAttribute.hpp"
#include "SomeGraphics/ResourcesCache.hpp"
#include "SomeGraphics/Skybox.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/Mesh.hpp"

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
    std::optional<std::unique_ptr<Texture>> texture_opt
        = Texture::create_cubemap(right, left, top, bottom, front, back);
    if (!texture_opt.has_value()) {
        return std::nullopt;
    }
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(std::vector<Vertex>({
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),

        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),

        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f,  1.0f)),
        Vertex(glm::vec3(-1.0f,  1.0f, -1.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f, -1.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,  1.0f)),
        Vertex(glm::vec3(1.0f, -1.0f,  1.0f)),
    }), std::initializer_list<VertexAttribute>({
        VertexAttributeType::Vec3,
    }), std::vector<uint>({
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35,
    }));
    return std::unique_ptr<Skybox>(new Skybox(std::move(program_opt.value()),
            std::move(mesh) , std::move(texture_opt.value())));
}

Skybox::~Skybox()
{
    is_instantiated = false;
}

const Program& Skybox::program() const
{
    return *m_program;
}

const Mesh& Skybox::mesh() const
{
    return *m_mesh;
}

const Texture& Skybox::texture() const
{
    return *m_texture;
}

bool Skybox::is_instantiated = false;

Skybox::Skybox(std::shared_ptr<Program>&& program,
    std::unique_ptr<Mesh>&& mesh, std::unique_ptr<Texture>&& texture) :
    m_program(std::move(program)),
    m_mesh(std::move(mesh)),
    m_texture(std::move(texture))
{
    assert(!is_instantiated && "Multiple instances are not allowed");
    is_instantiated = true;
}

}
