#include "SomeGraphics/IndexBuffer.hpp"

#include "glad/gl.h"

namespace sg {

IndexBuffer::IndexBuffer(std::vector<uint> indices) :
    m_count(indices.size())
{
    glCreateBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint),
        indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

uint IndexBuffer:: count() const
{
    return m_count;
}

}
