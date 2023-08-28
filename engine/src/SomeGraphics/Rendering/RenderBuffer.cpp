#include <utility>

#include "glm/ext/vector_int2.hpp"
#include "glad/gl.h"

#include "SomeGraphics/Rendering/RenderBuffer.hpp"

namespace sg {

RenderBuffer::RenderBuffer(const glm::ivec2& dimension)
{
    glCreateRenderbuffers(1, &m_renderer_id);
    glNamedRenderbufferStorage(m_renderer_id, GL_DEPTH24_STENCIL8, dimension.x, dimension.y);
}

RenderBuffer::RenderBuffer(RenderBuffer&& other)
{
    *this = std::move(other);
}

RenderBuffer& RenderBuffer::operator=(RenderBuffer&& other)
{
    if (this == &other) {
        return *this;
    }
    glDeleteRenderbuffers(1, &m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
    return *this;
}

RenderBuffer::~RenderBuffer()
{
    glDeleteRenderbuffers(1, &m_renderer_id);
}

void RenderBuffer::attach_to_framebuffer(uint32_t frame_buffer, GLenum attachment) const
{
    glNamedFramebufferRenderbuffer(frame_buffer, attachment, GL_RENDERBUFFER, m_renderer_id);
}

}
