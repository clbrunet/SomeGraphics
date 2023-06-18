#pragma once

#include <sys/types.h>
#include <vector>

#include "glad/gl.h"

namespace sg {

class IndexBuffer {
public:
    IndexBuffer() = delete;
    IndexBuffer(const std::vector<uint>& indices);
    IndexBuffer(IndexBuffer&& other);
    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other);
    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    ~IndexBuffer();

    uint count() const;
    GLenum format() const;
private:
    uint m_renderer_id = 0;
    uint m_count = 0;
    GLenum m_format = GL_UNSIGNED_INT;
};

}
