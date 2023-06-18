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
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
    return *this;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

}
