#pragma once

#include <sys/types.h>
#include <vector>

#include <glad/gl.h>

namespace sg {

class VertexBuffer {
public:
    template<typename T>
    VertexBuffer(const std::vector<T>& vertices)
    {
        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
    }
    ~VertexBuffer();
private:
    uint m_id = 0;
};

}
