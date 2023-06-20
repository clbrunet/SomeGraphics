#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <utility>
#include <vector>

#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/StbImageWrapper.hpp"

namespace sg {

Texture::Texture(const glm::vec2& dimension)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_renderer_id, 1, GL_RGB8, dimension.x, dimension.y);
    glTextureSubImage2D(m_renderer_id, 0, 0, 0, dimension.x, dimension.y,
        GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

std::optional<std::unique_ptr<Texture>> Texture::create_cubemap(const char* right,
    const char* left, const char* top, const char* bottom, const char* front, const char* back)
{
    std::array<const char*, 6> filenames({ right, left, top, bottom, front, back });
    std::vector<StbImageWrapper> images;
    images.reserve(6);
    for (const char* filename : filenames) {
        std::optional<StbImageWrapper> image_opt = StbImageWrapper::load(filename, 3);
        if (!image_opt.has_value()) {
            return std::nullopt;
        }
        images.emplace_back(std::move(image_opt.value()));
    }
    for (uint i = 1; i < 6; i++) {
        const StbImageWrapper& image = images[i];
        if (image.width() != images[0].width() || image.height() != images[0].height()) {
            std::cerr << "Cubemap creation error : not all textures have the same dimensions" << std::endl;
            return std::nullopt;
        }
    }
    return std::unique_ptr<Texture>(new Texture(images[0], images[1],
            images[2], images[3], images[4], images[5]));
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
    glDeleteTextures(1, &m_renderer_id);
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

Texture::Texture(const StbImageWrapper& right, const StbImageWrapper& left,
        const StbImageWrapper& top, const StbImageWrapper& bottom,
        const StbImageWrapper& front, const StbImageWrapper& back)
{
    std::array<const StbImageWrapper*, 6> images({ &right, &left, &top, &bottom, &front, &back });
#if SG_DEBUG
    const StbImageWrapper& first_image = *images[0];
    for (uint i = 0; i < 6; i++) {
        const StbImageWrapper& image = *images[i];
        if (image.width() != first_image.width() || image.height() != first_image.height()
            || image.channels_count() != 3) {
            assert(!"Cubemap textures must all have the same dimensions and must be in 3 channels");
        }
    }
#endif
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(m_renderer_id, 1, GL_RGB8, right.width(), right.height());
    for (int i = 0; i < 6; i++) {
        const StbImageWrapper& image = *images[i];
        glTextureSubImage3D(m_renderer_id, 0, 0, 0, i, image.width(), image.height(),
            1, GL_RGB, GL_UNSIGNED_BYTE, image.pixels());
    }
}

}
