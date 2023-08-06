#pragma once

#include <sys/types.h>
#include <memory>
#include <optional>

#include "glm/ext/vector_float2.hpp"
#include "assimp/texture.h"
#include "glad/gl.h"

namespace sg {

enum class ColorSpace {
    Srgb,
    Linear,
};

class StbImageWrapper;

class Texture {
public:
    Texture() = delete;
    Texture(const glm::vec2& dimensions);
    static std::unique_ptr<Texture> white_1px();
    static std::optional<std::unique_ptr<Texture>> from_ai_texture(const aiTexture& ai_texture, ColorSpace color_space);
    static std::optional<std::unique_ptr<Texture>> create_cubemap(const char* right,
        const char* left, const char* top, const char* bottom, const char* front, const char* back);
    Texture(Texture&& other);
    Texture(const Texture& other) = delete;
    Texture& operator=(Texture&& other);
    Texture& operator=(const Texture& other) = delete;
    ~Texture();

    void bind_to_unit(uint unit) const;
    uint renderer_id() const;
    void attach_to_framebuffer(uint frame_buffer, GLenum attachment) const;
private:
    uint m_renderer_id = 0;

    Texture(uint renderer_id);
    Texture(const aiTexture& ai_texture, ColorSpace color_space);
    Texture(const StbImageWrapper& image, ColorSpace color_space);
    Texture(const StbImageWrapper& right, const StbImageWrapper& left,
        const StbImageWrapper& top, const StbImageWrapper& bottom,
        const StbImageWrapper& front, const StbImageWrapper& back);

    static GLenum internal_format(uint channels_count, ColorSpace color_space);
    static GLenum format(uint channels_count);
};

}
