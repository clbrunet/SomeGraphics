#include "SomeGraphics/Rendering/VertexBuffer.hpp"

#include "glad/gl.h"

namespace sg {

VertexBuffer::VertexBuffer(VertexBuffer&& other)
{
    *this = std::move(other);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
    if (this == &other) {
        return *this;
    }
    glDeleteBuffers(1, &m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
    return *this;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

void VertexBuffer::bind_to_vertex_array(uint vertex_array, uint binding_index,
    intptr_t offset, uint stride) const
{
    glVertexArrayVertexBuffer(vertex_array, binding_index, m_renderer_id, offset, stride);
}

}
