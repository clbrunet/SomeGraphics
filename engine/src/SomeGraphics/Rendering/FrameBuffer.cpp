#include "glad/gl.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "SomeGraphics/Rendering/FrameBuffer.hpp"

namespace sg {

FrameBuffer::FrameBuffer(const glm::vec2& dimension) :
    m_color_texture(std::make_unique<Texture>(dimension)),
    m_depth_and_stencil_render_buffer(std::make_unique<RenderBuffer>(dimension))
{

    glCreateFramebuffers(1, &m_renderer_id);
    m_color_texture->attach_to_framebuffer(m_renderer_id, GL_COLOR_ATTACHMENT0);
    m_depth_and_stencil_render_buffer->attach_to_framebuffer(m_renderer_id,
        GL_DEPTH_STENCIL_ATTACHMENT);
    assert(glCheckNamedFramebufferStatus(m_renderer_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other)
{
    *this = std::move(other);
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other)
{
    if (this == &other) {
        return *this;
    }
    glDeleteFramebuffers(1, &m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    m_color_texture = std::move(other.m_color_texture);
    m_depth_and_stencil_render_buffer = std::move(other.m_depth_and_stencil_render_buffer);
    other.m_renderer_id = 0;
    return *this;
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
