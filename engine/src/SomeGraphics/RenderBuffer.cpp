#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

#include "SomeGraphics/RenderBuffer.hpp"

namespace sg {

RenderBuffer::RenderBuffer(const glm::vec2& dimension)
{
    glCreateRenderbuffers(1, &m_renderer_id);
    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dimension.x, dimension.y);
}

RenderBuffer::~RenderBuffer()
{
    glDeleteRenderbuffers(1, &m_renderer_id);
}

void RenderBuffer::bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderer_id);
}

void RenderBuffer::attach_to_framebuffer(GLenum attachment) const
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment,
        GL_RENDERBUFFER, m_renderer_id);
}

}
