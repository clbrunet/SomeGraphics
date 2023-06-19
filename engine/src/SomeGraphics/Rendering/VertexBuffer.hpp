#pragma once

#include <cstdint>
#include <sys/types.h>
#include <vector>

#include "glad/gl.h"

namespace sg {

class VertexBuffer {
public:
    VertexBuffer() = delete;
    template<typename T>
    VertexBuffer(const std::vector<T>& vertices)
    {
        glCreateBuffers(1, &m_renderer_id);
        glNamedBufferStorage(m_renderer_id, vertices.size() * sizeof(T), vertices.data(), 0);
    }
    VertexBuffer(VertexBuffer&& other);
    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(VertexBuffer&& other);
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    ~VertexBuffer();

    void bind_to_vertex_array(uint vertex_array, uint binding_index,
        intptr_t offset, uint stride) const;
private:
    uint m_renderer_id = 0;
};

}
