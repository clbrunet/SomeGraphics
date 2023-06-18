#include "glad/gl.h"

#include "SomeGraphics/Rendering/IndexBuffer.hpp"

namespace sg {

IndexBuffer::IndexBuffer(const std::vector<uint>& indices) :
    m_count(indices.size())
{
    glCreateBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other)
{
    *this = std::move(other);
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other)
{
    if (this == &other) {
        return *this;
    }
    m_renderer_id = other.m_renderer_id;
    m_count = other.m_count;
    m_format = other.m_format;
    other.m_renderer_id = 0;
    other.m_count = 0;
    return *this;
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
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
