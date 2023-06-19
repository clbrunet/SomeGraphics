#pragma once

#include <sys/types.h>

#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

namespace sg {

class Texture {
public:
    Texture() = delete;
    Texture(const glm::vec2& dimension);
    Texture(Texture&& other);
    Texture(const Texture& other) = delete;
    Texture& operator=(Texture&& other);
    Texture& operator=(const Texture& other) = delete;
    ~Texture();

    void bind_to_unit(uint unit) const;
    uint renderer_id() const;
    void attach_to_framebuffer(uint frame_buffer, GLenum attachment) const;
private:
    GLenum m_target;
    uint m_renderer_id = 0;
};

}
