#include "SomeGraphics/Mesh.hpp"

namespace sg {

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices)
{
    m_vertex_array = std::make_unique<VertexArray>(vertices,
        std::initializer_list<VertexAttribute>({ VertexAttribute(VertexAttributeType::Vec3), }),
        indices
    );
}

Mesh::~Mesh()
{
}

const VertexArray& Mesh::vertex_array() const
{
    return *m_vertex_array;
}

}
