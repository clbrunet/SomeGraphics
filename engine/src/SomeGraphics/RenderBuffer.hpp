#pragma once

#include <sys/types.h>

#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

namespace sg {

class RenderBuffer {
public:
    RenderBuffer(const glm::vec2& dimension);
    ~RenderBuffer();

    void bind() const;
    void attach_to_framebuffer(GLenum attachment) const;
private:
    uint m_renderer_id = 0;
};

};
