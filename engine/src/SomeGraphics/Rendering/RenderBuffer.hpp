#pragma once

#include <sys/types.h>

#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

namespace sg {

class RenderBuffer {
public:
    RenderBuffer() = delete;
    RenderBuffer(const glm::vec2& dimension);
    RenderBuffer(RenderBuffer&& other);
    RenderBuffer(const RenderBuffer& other) = delete;
    RenderBuffer& operator=(RenderBuffer&& other);
    RenderBuffer& operator=(const RenderBuffer& other) = delete;
    ~RenderBuffer();

    void attach_to_framebuffer(uint frame_buffer, GLenum attachment) const;
private:
    uint m_renderer_id = 0;
};

};
