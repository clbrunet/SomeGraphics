#include "SomeGraphics/Rendering/VertexArray.hpp"

namespace sg {

VertexArray::VertexArray(VertexArray&& other)
{
    *this = std::move(other);
}

VertexArray& VertexArray::operator=(VertexArray&& other)
{
    if (this == &other) {
        return *this;
    }
    m_renderer_id = other.m_renderer_id;
    m_vertex_buffer = std::move(other.m_vertex_buffer);
    m_index_buffer = std::move(other.m_index_buffer);
    other.m_renderer_id = 0;
    return *this;
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_renderer_id);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_renderer_id);
}

const IndexBuffer& VertexArray::index_buffer() const
{
    return *m_index_buffer;
}

}
