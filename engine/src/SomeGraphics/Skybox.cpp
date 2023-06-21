#include <cassert>
#include <optional>

#include "SomeGraphics/Skybox.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/Model.hpp"

namespace sg {

std::optional<std::unique_ptr<Skybox>> Skybox::create(const char* right, const char* left,
    const char* top, const char* bottom, const char* front, const char* back)
{
    std::optional<std::unique_ptr<Program>> program_opt
        = Program::create("engine/assets/shaders/skybox.vert", "engine/assets/shaders/skybox.frag");
    if (!program_opt.has_value()) {
        return std::nullopt;
    }
    std::optional<std::unique_ptr<Texture>> texture_opt
        = Texture::create_cubemap(right, left, top, bottom, front, back);
    if (!texture_opt.has_value()) {
        return std::nullopt;
    }
    std::vector<Mesh> meshes;
    meshes.emplace_back(std::vector<Vertex>({
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
    }),
    std::vector<uint>({
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35,
    }));
    return std::unique_ptr<Skybox>(new Skybox(std::move(program_opt.value()),
            std::make_unique<Model>(std::move(meshes)) , std::move(texture_opt.value())));
}

Skybox::~Skybox()
{
    is_instantiated = false;
}

const Program& Skybox::program() const
{
    return *m_program;
}

const Model& Skybox::model() const
{
    return *m_model;
}

const Texture& Skybox::texture() const
{
    return *m_texture;
}

bool Skybox::is_instantiated = false;

Skybox::Skybox(std::unique_ptr<Program>&& program,
    std::unique_ptr<Model>&& model, std::unique_ptr<Texture>&& texture) :
    m_program(std::move(program)),
    m_model(std::move(model)),
    m_texture(std::move(texture))
{
    assert(!is_instantiated && "Multiple instances are not allowed");
    is_instantiated = true;
}

}
