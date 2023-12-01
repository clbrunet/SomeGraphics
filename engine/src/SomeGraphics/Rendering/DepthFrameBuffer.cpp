#include <cassert>
#include <iostream>
#include <memory>

#include <glad/gl.h>

#include "SomeGraphics/Rendering/DepthFrameBuffer.hpp"
#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

DepthFrameBuffer::DepthFrameBuffer(const glm::ivec2& dimensions) :
    DepthFrameBuffer(Texture(dimensions, TextureFormat::Depth))
{
}

DepthFrameBuffer DepthFrameBuffer::create_cubemap(const glm::ivec2& dimensions)
{
    return DepthFrameBuffer(Texture::create_cubemap(dimensions, TextureFormat::Depth));
}

DepthFrameBuffer::DepthFrameBuffer(DepthFrameBuffer&& other) :
    m_depth_texture(std::move(other.m_depth_texture))
{
    if (this == &other) {
        return;
    }
    glDeleteFramebuffers(1, &m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
}

DepthFrameBuffer& DepthFrameBuffer::operator=(DepthFrameBuffer&& other)
{
    if (this == &other) {
        return *this;
    }
    glDeleteFramebuffers(1, &m_renderer_id);
    m_renderer_id = other.m_renderer_id;
    m_depth_texture = std::move(other.m_depth_texture);
    other.m_renderer_id = 0;
    return *this;
}

DepthFrameBuffer::~DepthFrameBuffer()
{
    glDeleteFramebuffers(1, &m_renderer_id);
}

void DepthFrameBuffer::attach_face(CubemapFace face) const
{
    m_depth_texture.attach_face_to_framebuffer(m_renderer_id, GL_DEPTH_ATTACHMENT, face);
    assert(glCheckNamedFramebufferStatus(m_renderer_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void DepthFrameBuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
}

const Texture& DepthFrameBuffer::depth_texture() const
{
    return m_depth_texture;
}

DepthFrameBuffer::DepthFrameBuffer(Texture depth_texture) :
    m_depth_texture(std::move(depth_texture))
{
    glCreateFramebuffers(1, &m_renderer_id);
    glNamedFramebufferDrawBuffer(m_renderer_id, GL_NONE);
    glNamedFramebufferReadBuffer(m_renderer_id, GL_NONE);
    m_depth_texture.attach_to_framebuffer(m_renderer_id, GL_DEPTH_ATTACHMENT);
    assert(glCheckNamedFramebufferStatus(m_renderer_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

}
