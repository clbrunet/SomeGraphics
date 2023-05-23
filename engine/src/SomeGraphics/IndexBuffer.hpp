#pragma once

#include <sys/types.h>
#include <vector>

#include <glad/gl.h>

namespace sg {

class IndexBuffer {
public:
    IndexBuffer(const std::vector<uint>& indices);
    ~IndexBuffer();

    uint count() const;
    GLenum format() const;
private:
    uint m_id = 0;
    uint m_count = 0;
    GLenum m_format = GL_UNSIGNED_INT;
};

}
