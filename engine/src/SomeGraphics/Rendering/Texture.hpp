#pragma once

#include <sys/types.h>
#include <memory>
#include <optional>

#include <glm/ext/vector_int2.hpp>
#include <assimp/texture.h>
#include <glad/gl.h>
#include <imgui.h>

namespace sg {

enum class ColorSpace {
    Srgb,
    Linear,
    Varying,
};

class StbImageWrapper;

class Texture {
public:
    Texture() = delete;
    Texture(const glm::ivec2& dimensions);
    static std::unique_ptr<Texture> white_1px();
    static std::optional<std::unique_ptr<Texture>> from_ai_texture(const aiTexture& ai_texture, ColorSpace color_space);
    static std::optional<std::unique_ptr<Texture>> create_cubemap(const char* right,
        const char* left, const char* top, const char* bottom, const char* front, const char* back);
    Texture(Texture&& other);
    Texture(const Texture& other) = delete;
    Texture& operator=(Texture&& other);
    Texture& operator=(const Texture& other) = delete;
    ~Texture();

    ColorSpace color_space() const;
    void bind_to_unit(uint32_t unit) const;
    ImTextureID imgui_texture_id() const;
    void attach_to_framebuffer(uint32_t frame_buffer, GLenum attachment) const;
private:
    uint32_t m_renderer_id = 0;
    ColorSpace m_color_space;

    Texture(uint32_t renderer_id, ColorSpace color_space);
    Texture(const aiTexture& ai_texture, ColorSpace color_space);
    Texture(const StbImageWrapper& image, ColorSpace color_space);
    Texture(const StbImageWrapper& right, const StbImageWrapper& left,
        const StbImageWrapper& top, const StbImageWrapper& bottom,
        const StbImageWrapper& front, const StbImageWrapper& back);

    static GLenum internal_format(uint32_t channels_count, ColorSpace color_space);
    static GLenum format(uint32_t channels_count);
};

}
