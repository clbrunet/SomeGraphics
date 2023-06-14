#include "SomeGraphics/VertexArray.hpp"

namespace sg {

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
