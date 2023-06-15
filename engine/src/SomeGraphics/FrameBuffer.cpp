#include "glad/gl.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "SomeGraphics/FrameBuffer.hpp"

namespace sg {

FrameBuffer::FrameBuffer(const glm::vec2& dimension)
{
    glCreateFramebuffers(1, &m_renderer_id);
    bind();
    m_color_texture = std::make_unique<Texture>(dimension);
    m_color_texture->attach_to_framebuffer(GL_COLOR_ATTACHMENT0);
    m_depth_and_stencil_render_buffer = std::make_unique<RenderBuffer>(dimension);
    m_depth_and_stencil_render_buffer->attach_to_framebuffer(GL_DEPTH_STENCIL_ATTACHMENT);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

FrameBuffer::~FrameBuffer()
{
    glDeleteFramebuffers(1, &m_renderer_id);
}

void FrameBuffer::bind_default()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
}

const Texture& FrameBuffer::color_texture() const
{
    return *m_color_texture;
}

}
