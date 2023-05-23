#include <glad/gl.h>

#include "SomeGraphics/IndexBuffer.hpp"

namespace sg {

IndexBuffer::IndexBuffer(const std::vector<uint>& indices) :
    m_count(indices.size())
{
    glCreateBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

uint IndexBuffer::count() const
{
    return m_count;
}

GLenum IndexBuffer::format() const
{
    return m_format;
}

}
