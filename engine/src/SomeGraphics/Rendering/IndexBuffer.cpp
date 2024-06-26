#include <iostream>

#include <glad/gl.h>

#include "SomeGraphics/Rendering/IndexBuffer.hpp"

namespace sg {

IndexBuffer::IndexBuffer(std::span<const uint32_t> indices) :
    m_count((uint32_t)indices.size())
{
    glCreateBuffers(1, &m_renderer_id);
    glNamedBufferStorage(m_renderer_id, m_count * sizeof(uint32_t), indices.data(), 0);
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
    glDeleteBuffers(1, &m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    m_count = other.m_count;
    m_format = other.m_format;
    other.m_renderer_id = 0;
    return *this;
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

uint32_t IndexBuffer::count() const
{
    return m_count;
}

GLenum IndexBuffer::format() const
{
    return m_format;
}

void IndexBuffer::bind_to_vertex_array(uint32_t vertex_array) const
{
    glVertexArrayElementBuffer(vertex_array, m_renderer_id);
}

}
