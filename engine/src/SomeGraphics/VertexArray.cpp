#include "SomeGraphics/VertexArray.hpp"

namespace sg {

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

const IndexBuffer& VertexArray::index_buffer() const
{
    return *m_index_buffer;
}

}
