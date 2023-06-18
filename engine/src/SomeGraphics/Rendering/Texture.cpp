#include <utility>

#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

#include "SomeGraphics/Rendering/Texture.hpp"

namespace sg {

Texture::Texture(const glm::vec2& dimension) :
    m_target(GL_TEXTURE_2D)
{
    glCreateTextures(m_target, 1, &m_renderer_id);
    bind();
    glTexImage2D(m_target, 0, GL_RGB, dimension.x, dimension.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

void Texture::bind() const
{
    glBindTexture(m_target, m_renderer_id);
}

void Texture::bind_to_unit(uint unit) const
{
    glBindTextureUnit(unit, m_renderer_id);
}

uint Texture::renderer_id() const
{
    return m_renderer_id;
}

void Texture::attach_to_framebuffer(GLenum attachment) const
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, m_target, m_renderer_id, 0);
}

}
