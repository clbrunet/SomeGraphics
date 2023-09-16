#pragma once

#include <optional>
#include <memory>

#include <glm/ext/vector_float2.hpp>

namespace sg {

class Program;
class Mesh;
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
    const std::unique_ptr<Mesh>& quad() const;
private:
    std::shared_ptr<Program> m_program;
    std::unique_ptr<Mesh> m_quad;

    struct Vertex {
        glm::vec2 position;
        glm::vec2 texture_coordinates;

        Vertex() = delete;
        Vertex(glm::vec2 position, glm::vec2 texture_coordinates) :
            position(std::move(position)),
            texture_coordinates(std::move(texture_coordinates))
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
