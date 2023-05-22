#include "SomeGraphics/VertexBuffer.hpp"

#include <glad/gl.h>

namespace sg {

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

}
