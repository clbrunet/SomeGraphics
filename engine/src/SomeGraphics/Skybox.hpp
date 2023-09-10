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

    const std::shared_ptr<Program>& program() const;
    const std::unique_ptr<Mesh>& mesh() const;
    const std::shared_ptr<Texture>& cubemap() const;
private:
    std::shared_ptr<Program> m_program;
    std::unique_ptr<Mesh> m_mesh;
    std::shared_ptr<Texture> m_cubemap;
    static bool is_instantiated;

    Skybox(std::shared_ptr<Program>&& program,
        std::unique_ptr<Mesh>&& mesh, std::shared_ptr<Texture>&& cubemap);

    struct Vertex {
        glm::vec3 position;

        Vertex() = delete;
        Vertex(glm::vec3 position) :
            position(std::move(position))
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
