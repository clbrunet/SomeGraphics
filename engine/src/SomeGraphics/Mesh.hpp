#pragma once

#include <memory>
#include <sys/types.h>
#include <vector>

#include "glm/ext/vector_float3.hpp"

#include "SomeGraphics/Rendering/VertexArray.hpp"

namespace sg {

struct Vertex {
    glm::vec3 position;

    Vertex(const glm::vec3& position) :
        position(position)
    {
    }
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices);
    Mesh(Mesh&& other) = default;
    ~Mesh();

    const VertexArray& vertex_array() const;
private:
    std::unique_ptr<VertexArray> m_vertex_array;
};

}
