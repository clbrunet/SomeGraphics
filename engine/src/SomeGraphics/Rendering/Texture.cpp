#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <sys/types.h>
#include <utility>
#include <vector>

#include "assimp/texture.h"
#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/StbImageWrapper.hpp"

namespace sg {

Texture::Texture(const glm::vec2& dimensions)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(m_renderer_id, 1, GL_RGB8, dimensions.x, dimensions.y);
    glTextureSubImage2D(m_renderer_id, 0, 0, 0, dimensions.x, dimensions.y,
        GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

std::unique_ptr<Texture> Texture::white_1px()
{
    uint renderer_id;
    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
    glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(renderer_id, 1, GL_RGB8, 1, 1);
    u_char white[3] = { 255, 255, 255 };
    glTextureSubImage2D(renderer_id, 0, 0, 0, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, white);
    return std::unique_ptr<Texture>(new Texture(renderer_id));
}

std::optional<std::unique_ptr<Texture>> Texture::from_ai_texture(const aiTexture& ai_texture)
{
    if (ai_texture.mHeight != 0) {
        return std::unique_ptr<Texture>(new Texture(ai_texture));
    }
    std::optional<StbImageWrapper> image_opt = StbImageWrapper::load_from_memory(
        (const u_char*)ai_texture.pcData, ai_texture.mWidth);
    if (!image_opt.has_value()) {
        return std::nullopt;
    }
    return std::unique_ptr<Texture>(new Texture(image_opt.value()));
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

Texture::Texture(uint renderer_id) :
    m_renderer_id(renderer_id)
{
}

Texture::Texture(const aiTexture& ai_texture)
{
    assert(ai_texture.mHeight != 0);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_renderer_id, 1, GL_RGBA8, ai_texture.mWidth, ai_texture.mHeight);
    glTextureSubImage2D(m_renderer_id, 0, 0, 0, ai_texture.mWidth, ai_texture.mHeight,
        GL_RGBA, GL_UNSIGNED_BYTE, ai_texture.pcData);
}

Texture::Texture(const StbImageWrapper& image)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_renderer_id, 1, internal_format(image), image.width(), image.height());
    glTextureSubImage2D(m_renderer_id, 0, 0, 0, image.width(), image.height(),
        format(image), GL_UNSIGNED_BYTE, image.pixels());
}

Texture::Texture(const StbImageWrapper& right, const StbImageWrapper& left,
        const StbImageWrapper& top, const StbImageWrapper& bottom,
        const StbImageWrapper& front, const StbImageWrapper& back)
{
    std::array<const StbImageWrapper*, 6> images({ &right, &left, &top, &bottom, &back, &front });
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
    glTextureStorage2D(m_renderer_id, 1, GL_SRGB8, right.width(), right.height());
    for (int i = 0; i < 6; i++) {
        const StbImageWrapper& image = *images[i];
        glTextureSubImage3D(m_renderer_id, 0, 0, 0, i, image.width(), image.height(),
            1, GL_RGB, GL_UNSIGNED_BYTE, image.pixels());
    }
}

GLenum Texture::internal_format(const StbImageWrapper& image)
{
    switch (image.channels_count()) {
        case 3:
            return GL_SRGB8;
        case 4:
            return GL_SRGB8_ALPHA8;
        default:
            assert(false);
            return GL_NONE;
    }
}

GLenum Texture::format(const StbImageWrapper& image)
{
    switch (image.channels_count()) {
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            assert(false);
            return GL_NONE;
    }
}

}
