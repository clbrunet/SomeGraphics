#include "SomeGraphics/Rendering/VertexBuffer.hpp"

#include "glad/gl.h"

namespace sg {

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_renderer_id);
}

}
