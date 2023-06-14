#pragma once

#include <sys/types.h>

#include "SomeGraphics/Texture.hpp"
#include "glm/ext/vector_float2.hpp"

namespace sg {

class FrameBuffer {
public:
    FrameBuffer(const glm::vec2& dimension);
    ~FrameBuffer();

    static void bind_default();
    void bind() const;
    const Texture& color_texture() const;
private:
    Texture m_color_texture;
    uint m_renderer_id = 0;
};

}
