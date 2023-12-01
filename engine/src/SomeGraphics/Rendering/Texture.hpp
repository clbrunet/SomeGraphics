#pragma once

#include <cstdint>
#include <sys/types.h>
#include <optional>

#include <glm/ext/vector_int2.hpp>
#include <assimp/texture.h>
#include <glad/gl.h>
#include <imgui.h>

namespace sg {

enum class TextureFormat {
    Rgb,
    F16Rgb,
    Depth,
};

enum class ColorSpace {
    Unknown,
    Srgb,
    Linear,
};

enum CubemapFace {
    Begin,
    PositiveX = Begin,
    NegativeX,
    PositiveY,
    NegativeY,
    PositiveZ,
    NegativeZ,
    End,
};

class StbImageWrapper;

class Texture {
public:
    Texture() = delete;
    Texture(glm::ivec2 dimensions, TextureFormat format);
    static Texture white_1px();
    static std::optional<Texture> from_ai_texture(const aiTexture& ai_texture, ColorSpace color_space);
    static std::optional<Texture> create_cubemap(const char* right,
        const char* left, const char* top, const char* bottom, const char* front, const char* back);
    static Texture create_cubemap(glm::ivec2 dimensions, TextureFormat format);
    Texture(Texture&& other);
    Texture(const Texture& other) = delete;
    Texture& operator=(Texture&& other);
    Texture& operator=(const Texture& other) = delete;
    ~Texture();

    ColorSpace color_space() const;
    void bind_to_unit(uint8_t unit) const;
    ImTextureID imgui_texture_id() const;
#if SG_ENGINE
    void attach_to_framebuffer(uint32_t frame_buffer, GLenum attachment) const;
    void attach_face_to_framebuffer(uint32_t frame_buffer, GLenum attachment, CubemapFace face) const;
#endif
private:
    uint32_t m_renderer_id = 0;
    ColorSpace m_color_space;

    Texture(uint32_t renderer_id, ColorSpace color_space);
    Texture(const aiTexture& ai_texture, ColorSpace color_space);
    Texture(const StbImageWrapper& image, ColorSpace color_space);
    Texture(const StbImageWrapper& right, const StbImageWrapper& left,
        const StbImageWrapper& top, const StbImageWrapper& bottom,
        const StbImageWrapper& front, const StbImageWrapper& back);

    static GLenum sized_internal_format(uint32_t channels_count, ColorSpace color_space);
    static GLenum sized_internal_format(TextureFormat format);
    static GLenum internal_format(uint32_t channels_count);
    static GLenum internal_format(TextureFormat format);
};

}
