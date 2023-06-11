#pragma once

#include <sys/types.h>

#include "glm/ext/vector_float2.hpp"
#include "glad/gl.h"

namespace sg {

class Texture {
public:
    Texture(const glm::vec2& dimension);
    ~Texture();

    void bind() const;

    void attach_to_framebuffer(GLenum attachment) const;
private:
    GLenum m_target;
    uint m_id = 0;
};

}
