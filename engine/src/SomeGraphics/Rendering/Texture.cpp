#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <optional>
#include <sys/types.h>
#include <utility>
#include <vector>

#include <assimp/texture.h>
#include <glm/ext/vector_int2.hpp>
#include <glad/gl.h>
#include <imgui.h>

#include "SomeGraphics/Rendering/Texture.hpp"
#include "SomeGraphics/StbImageWrapper.hpp"

namespace sg {

Texture::Texture(glm::ivec2 dimensions, TextureFormat format) :
    m_color_space(ColorSpace::Unknown)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(m_renderer_id, 1, sized_internal_format(format), dimensions.x, dimensions.y);
}

Texture Texture::white_1px()
{
    uint32_t renderer_id;
    glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
    glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(renderer_id, 1, GL_RGB8, 1, 1);
    uint8_t white[3] = { 255, 255, 255 };
    glTextureSubImage2D(renderer_id, 0, 0, 0, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, white);
    return Texture(renderer_id, ColorSpace::Linear);
}

std::optional<Texture> Texture::from_ai_texture(const aiTexture& ai_texture,
    ColorSpace color_space)
{
    if (ai_texture.mHeight != 0) {
        return Texture(ai_texture, color_space);
    }
    std::optional<StbImageWrapper> image_opt = StbImageWrapper::load_from_memory(
        (const uint8_t*)ai_texture.pcData, ai_texture.mWidth);
    if (!image_opt.has_value()) {
        return std::nullopt;
    }
    return Texture(image_opt.value(), color_space);
}

std::optional<Texture> Texture::create_cubemap(const char* right,
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
    for (uint32_t i = 1; i < 6; i++) {
        const StbImageWrapper& image = images[i];
        if (image.width() != images[0].width() || image.height() != images[0].height()) {
            std::cerr << "Cubemap creation error : not all textures have the same dimensions\n";
            return std::nullopt;
        }
    }
    return Texture(images[0], images[1], images[2], images[3], images[4], images[5]);
}

Texture Texture::create_cubemap(glm::ivec2 dimensions, TextureFormat format)
{
    uint32_t renderer_id;
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &renderer_id);
    glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(renderer_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(renderer_id, 1, sized_internal_format(format), dimensions.x, dimensions.y);
    return Texture(renderer_id, ColorSpace::Unknown);
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

ColorSpace Texture::color_space() const
{
    return m_color_space;
}

void Texture::bind_to_unit(uint8_t unit) const
{
    glBindTextureUnit(unit, m_renderer_id);
}

ImTextureID Texture::imgui_texture_id() const
{
    return (void*)(intptr_t)m_renderer_id;
}

void Texture::attach_to_framebuffer(uint32_t frame_buffer, GLenum attachment) const
{
    glNamedFramebufferTexture(frame_buffer, attachment, m_renderer_id, 0);
}

void Texture::attach_face_to_framebuffer(uint32_t frame_buffer,
    GLenum attachment, CubemapFace face) const
{
    glNamedFramebufferTextureLayer(frame_buffer, attachment, m_renderer_id, 0, (int)face);
}

Texture::Texture(uint32_t renderer_id, ColorSpace color_space) :
    m_renderer_id(renderer_id),
    m_color_space(color_space)
{
}

Texture::Texture(const aiTexture& ai_texture, ColorSpace color_space) :
    m_color_space(color_space)
{
    assert(ai_texture.mHeight != 0);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_renderer_id, 1, sized_internal_format(4, color_space),
        ai_texture.mWidth, ai_texture.mHeight);
    glTextureSubImage2D(m_renderer_id, 0, 0, 0, ai_texture.mWidth, ai_texture.mHeight,
        GL_RGBA, GL_UNSIGNED_BYTE, ai_texture.pcData);
}

Texture::Texture(const StbImageWrapper& image, ColorSpace color_space) :
    m_color_space(color_space)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(m_renderer_id, 1, sized_internal_format(image.channels_count(), color_space),
        image.width(), image.height());
    glTextureSubImage2D(m_renderer_id, 0, 0, 0, image.width(), image.height(),
        internal_format(image.channels_count()), GL_UNSIGNED_BYTE, image.pixels());
}

Texture::Texture(const StbImageWrapper& right, const StbImageWrapper& left,
    const StbImageWrapper& top, const StbImageWrapper& bottom,
    const StbImageWrapper& front, const StbImageWrapper& back) :
    m_color_space(ColorSpace::Srgb)
{
    std::array<const StbImageWrapper*, 6> images({ &right, &left, &top, &bottom, &back, &front });
#if SG_DEBUG
    const StbImageWrapper& first_image = *images[0];
    for (uint32_t i = 0; i < 6; i++) {
        const StbImageWrapper& image = *images[i];
        if (image.width() != first_image.width() || image.height() != first_image.height()
            || image.channels_count() != 3) {
            assert(!(bool)"Cubemap textures must all have the same dimensions and must be in 3 channels");
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
    for (uint32_t i = 0; i < 6; i++) {
        const StbImageWrapper& image = *images[i];
        glTextureSubImage3D(m_renderer_id, 0, 0, 0, i, image.width(), image.height(),
            1, GL_RGB, GL_UNSIGNED_BYTE, image.pixels());
    }
}

GLenum Texture::sized_internal_format(uint32_t channels_count, ColorSpace color_space)
{
    switch (color_space) {
        case ColorSpace::Unknown:
            break;
        case ColorSpace::Srgb:
            switch (channels_count) {
                case 3:
                    return GL_SRGB8;
                case 4:
                    return GL_SRGB8_ALPHA8;
            }
            break;
        case ColorSpace::Linear:
            switch (channels_count) {
                case 3:
                    return GL_RGB8;
                case 4:
                    return GL_RGBA8;
            }
            break;
    }
    assert(false);
    return GL_NONE;
}

GLenum Texture::sized_internal_format(TextureFormat format)
{
    switch (format) {
        case TextureFormat::Rgb:
            return GL_RGB8;
        case TextureFormat::F16Rgb:
            return GL_RGB16F;
        case TextureFormat::Depth:
            return GL_DEPTH_COMPONENT32F;
    }
    assert(false);
    return GL_NONE;
}

GLenum Texture::internal_format(uint32_t channels_count)
{
    switch (channels_count) {
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
    }
    assert(false);
    return GL_NONE;
}

GLenum Texture::internal_format(TextureFormat format)
{
    switch (format) {
        case TextureFormat::Rgb:
        case TextureFormat::F16Rgb:
            return GL_RGB;
        case TextureFormat::Depth:
            return GL_DEPTH_COMPONENT;
    }
    assert(false);
    return GL_NONE;
}

}
