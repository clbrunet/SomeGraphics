#pragma once

#include <memory>
#include <sys/types.h>
#include <vector>

#include "glad/gl.h"

#include "SomeGraphics/Rendering/VertexBuffer.hpp"
#include "SomeGraphics/Rendering/IndexBuffer.hpp"
#include "SomeGraphics/Rendering/VertexAttribute.hpp"

namespace sg {

class VertexAttribute;
class VertexBuffer;

class VertexArray {
public:
    VertexArray() = delete;
    template<typename T>
    VertexArray(const std::vector<T>& vertices, std::initializer_list<VertexAttribute> attributes,
        const std::vector<uint> indices) :
        m_vertex_buffer(std::make_unique<VertexBuffer>(vertices)),
        m_index_buffer(std::make_unique<IndexBuffer>(indices))
    {
        glCreateVertexArrays(1, &m_renderer_id);
        uint stride = 0;
        for (const VertexAttribute& attribute : attributes) {
            stride += attribute.size();
        }
        m_vertex_buffer->bind_to_vertex_array(m_renderer_id, 0, 0, stride);
        m_index_buffer->bind_to_vertex_array(m_renderer_id);
        uint index = 0;
        uint offset = 0;
        for (const VertexAttribute& attribute : attributes) {
            glEnableVertexArrayAttrib(m_renderer_id, index);
            glVertexArrayAttribFormat(m_renderer_id, index, attribute.count(),
                attribute.type(), GL_FALSE, offset);
            glVertexArrayAttribBinding(m_renderer_id, index, 0);
            index++;
            offset += attribute.size();
        }
    }
    VertexArray(VertexArray&& other);
    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(VertexArray&& other);
    VertexArray& operator=(const VertexArray& other) = delete;
    ~VertexArray();

    void bind() const;
    const std::unique_ptr<IndexBuffer>& index_buffer() const;
private:
    uint m_renderer_id = 0;
    std::unique_ptr<VertexBuffer> m_vertex_buffer;
    std::unique_ptr<IndexBuffer> m_index_buffer;
};

}
