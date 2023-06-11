#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

#include "SomeGraphics/Texture.hpp"

namespace sg {

Texture::Texture(const glm::vec2& dimension) :
    m_target(GL_TEXTURE_2D)
{
    glCreateTextures(m_target, 1, &m_id);
    bind();
    glTexImage2D(m_target, 0, GL_RGB, dimension.x, dimension.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

Texture::~Texture()
{
    glDeleteFramebuffers(1, &m_id);
}

void Texture::bind() const
{
    glBindTexture(m_target, m_id);
}

void Texture::attach_to_framebuffer(GLenum attachment) const
{
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, m_target, m_id, 0);
}

}
