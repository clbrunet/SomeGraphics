#pragma once

#include <memory>
#include <sys/types.h>
#include <vector>

#include "glm/ext/vector_float3.hpp"

#include "SomeGraphics/Rendering/VertexArray.hpp"

namespace sg {

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

class Mesh {
public:
    Mesh() = delete;
    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices);
    Mesh(Mesh&& other) = default;
    Mesh(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) = default;
    Mesh& operator=(const Mesh& other) = delete;
    ~Mesh();

    const VertexArray& vertex_array() const;
private:
    std::unique_ptr<VertexArray> m_vertex_array;
};

}
