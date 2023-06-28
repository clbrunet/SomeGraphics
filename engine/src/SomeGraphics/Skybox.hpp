#pragma once

#include <optional>
#include <memory>

#include "glm/ext/vector_float3.hpp"

namespace sg {

class Program;
class Mesh;
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

    const Program& program() const;
    const Mesh& mesh() const;
    const Texture& texture() const;
private:
    std::unique_ptr<Program> m_program;
    std::unique_ptr<Mesh> m_mesh;
    std::unique_ptr<Texture> m_texture;
    static bool is_instantiated;

    Skybox(std::unique_ptr<Program>&& program,
        std::unique_ptr<Mesh>&& mesh, std::unique_ptr<Texture>&& texture);

    struct Vertex {
        glm::vec3 position;

        Vertex() = delete;
        Vertex(const glm::vec3& position) :
            position(position)
        {
        }
        Vertex(Vertex&& other) = default;
        Vertex(const Vertex& other) = default;
        Vertex& operator=(Vertex&& other) = default;
        Vertex& operator=(const Vertex& other) = default;
        ~Vertex() = default;
    };
};

}
