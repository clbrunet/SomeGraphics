#include "SomeGraphics/Mesh.hpp"

namespace sg {

Mesh::~Mesh()
{
}

const VertexArray& Mesh::vertex_array() const
{
    return *m_vertex_array;
}

}
