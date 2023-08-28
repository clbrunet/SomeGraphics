#pragma once

#include <sys/types.h>

#include "glm/ext/vector_int2.hpp"
#include "glad/gl.h"

namespace sg {

class RenderBuffer {
public:
    RenderBuffer() = delete;
    RenderBuffer(const glm::ivec2& dimension);
    RenderBuffer(RenderBuffer&& other);
    RenderBuffer(const RenderBuffer& other) = delete;
    RenderBuffer& operator=(RenderBuffer&& other);
    RenderBuffer& operator=(const RenderBuffer& other) = delete;
    ~RenderBuffer();

    void attach_to_framebuffer(uint32_t frame_buffer, GLenum attachment) const;
private:
    uint32_t m_renderer_id = 0;
};

};
