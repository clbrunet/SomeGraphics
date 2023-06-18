#pragma once

#include <memory>
#include <sys/types.h>
#include <vector>

#include "glad/gl.h"

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace sg {

class VertexAttribute;

class VertexArray {
public:
    VertexArray() = delete;
    template<typename T>
    VertexArray(const std::vector<T>& vertices, std::initializer_list<VertexAttribute> attributes,
        const std::vector<uint> indices)
    {
        glCreateVertexArrays(1, &m_renderer_id);
        bind();
        m_vertex_buffer = std::make_unique<VertexBuffer>(vertices, attributes);
        m_index_buffer = std::make_unique<IndexBuffer>(indices);
    }
    VertexArray(VertexArray&& other);
    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(VertexArray&& other);
    VertexArray& operator=(const VertexArray& other) = delete;
    ~VertexArray();

    void bind() const;
    const IndexBuffer& index_buffer() const;
private:
    uint m_renderer_id = 0;
    std::unique_ptr<VertexBuffer> m_vertex_buffer;
    std::unique_ptr<IndexBuffer> m_index_buffer;
};

}
