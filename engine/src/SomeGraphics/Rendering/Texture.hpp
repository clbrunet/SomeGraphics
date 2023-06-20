#pragma once

#include <sys/types.h>
#include <memory>
#include <optional>

#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

namespace sg {

class StbImageWrapper;

class Texture {
public:
    Texture() = delete;
    Texture(const glm::vec2& dimension);
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

    Texture(const StbImageWrapper& right, const StbImageWrapper& left,
        const StbImageWrapper& top, const StbImageWrapper& bottom,
        const StbImageWrapper& front, const StbImageWrapper& back);
};

}