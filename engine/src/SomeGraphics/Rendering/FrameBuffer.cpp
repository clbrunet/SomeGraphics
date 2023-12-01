#include <cassert>
#include <iostream>
#include <memory>

#include <glad/gl.h>

#include "SomeGraphics/Rendering/FrameBuffer.hpp"

namespace sg {

FrameBuffer::FrameBuffer(const glm::ivec2& dimensions, TextureFormat texture_format) :
    m_color_texture(dimensions, texture_format),
    m_depth_and_stencil_render_buffer(dimensions)
{

    glCreateFramebuffers(1, &m_renderer_id);
    m_color_texture.attach_to_framebuffer(m_renderer_id, GL_COLOR_ATTACHMENT0);
    m_depth_and_stencil_render_buffer.attach_to_framebuffer(m_renderer_id,
        GL_DEPTH_STENCIL_ATTACHMENT);
    assert(glCheckNamedFramebufferStatus(m_renderer_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

FrameBuffer::FrameBuffer(FrameBuffer&& other) :
    m_color_texture(std::move(other.m_color_texture)),
    m_depth_and_stencil_render_buffer(std::move(other.m_depth_and_stencil_render_buffer))
{
    if (this == &other) {
        return;
    }
    glDeleteFramebuffers(1, &m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
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
    return m_color_texture;
}

}
