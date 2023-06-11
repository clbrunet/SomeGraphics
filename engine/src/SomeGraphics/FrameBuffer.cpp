#include "glad/gl.h"

#include "SomeGraphics/FrameBuffer.hpp"
#include <cassert>
#include <iostream>

namespace sg {

FrameBuffer::FrameBuffer(const glm::vec2& dimension) :
    m_color_texture(dimension)
{
    glCreateFramebuffers(1, &m_id);
    bind();
    m_color_texture.attach_to_framebuffer(GL_COLOR_ATTACHMENT0);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    bind_default();
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_id);
}

void FrameBuffer::bind_default()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

}
