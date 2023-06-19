#include <utility>

#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

Texture::Texture(const glm::vec2& dimension) :
    m_target(GL_TEXTURE_2D)
{
    glCreateTextures(m_target, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_renderer_id, 1, GL_RGB8, dimension.x, dimension.y);
    glTextureSubImage2D(m_renderer_id, 0, 0, 0, dimension.x, dimension.y,
        GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

Texture::Texture(Texture&& other)
{
    *this = std::move(other);
}

Texture& Texture::operator=(Texture&& other)
{
    if (this == &other) {
        return *this;
    }
    m_target = other.m_target;
    m_renderer_id = other.m_renderer_id;
    other.m_renderer_id = 0;
    return *this;
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_renderer_id);
}

void Texture::bind_to_unit(uint unit) const
{
    glBindTextureUnit(unit, m_renderer_id);
}

uint Texture::renderer_id() const
{
    return m_renderer_id;
}

void Texture::attach_to_framebuffer(uint frame_buffer, GLenum attachment) const
{
    glNamedFramebufferTexture(frame_buffer, attachment, m_renderer_id, 0);
}

}
