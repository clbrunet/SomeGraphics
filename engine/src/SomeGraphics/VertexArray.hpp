#pragma once

#include <memory>
#include <sys/types.h>
#include <vector>

#include <glad/gl.h>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace sg {

class VertexAttribute;

class VertexArray {
public:
    template<typename T>
    VertexArray(const std::vector<T>& vertices, std::initializer_list<VertexAttribute> attributes,
        const std::vector<uint> indices)
    {
        glCreateVertexArrays(1, &m_id);
        bind();
        m_vertex_buffer = std::make_unique<VertexBuffer>(vertices, attributes);
        m_index_buffer = std::make_unique<IndexBuffer>(indices);
    }
    ~VertexArray();

    void bind() const;
    const IndexBuffer& index_buffer() const;
private:
    uint m_id = 0;
    std::unique_ptr<VertexBuffer> m_vertex_buffer;
    std::unique_ptr<IndexBuffer> m_index_buffer;
};

}
