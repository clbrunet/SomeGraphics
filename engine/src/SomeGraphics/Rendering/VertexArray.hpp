#pragma once

#include <sys/types.h>
#include <memory>
#include <vector>
#include <span>

#include <glad/gl.h>

#include "SomeGraphics/Rendering/VertexBuffer.hpp"
#include "SomeGraphics/Rendering/IndexBuffer.hpp"
#include "SomeGraphics/Rendering/VertexAttribute.hpp"

namespace sg {

class VertexAttribute;
class VertexBuffer;

class VertexArray {
public:
    VertexArray() = delete;
    template<typename Vertex>
    VertexArray(std::span<const Vertex> vertices, std::span<const VertexAttribute> attributes,
        std::span<const uint32_t> indices) :
        m_vertex_buffer(std::make_unique<VertexBuffer>(vertices)),
        m_index_buffer(std::make_unique<IndexBuffer>(indices))
    {
        glCreateVertexArrays(1, &m_renderer_id);
        bind(); // required for glVertexArrayAttribIFormat to work on Intel GPUs
        uint32_t stride = 0;
        for (const VertexAttribute& attribute : attributes) {
            stride += attribute.size();
        }
        m_vertex_buffer->bind_to_vertex_array(m_renderer_id, 0, 0, stride);
        m_index_buffer->bind_to_vertex_array(m_renderer_id);
        uint32_t index = 0;
        uint32_t offset = 0;
        for (const VertexAttribute& attribute : attributes) {
            glEnableVertexArrayAttrib(m_renderer_id, index);
            if (attribute.is_integer()) {
                glVertexArrayAttribIFormat(m_renderer_id, index, attribute.count(),
                    attribute.type(), offset);
            } else {
                glVertexArrayAttribFormat(m_renderer_id, index, attribute.count(),
                    attribute.type(), GL_FALSE, offset);
            }
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
    uint32_t m_renderer_id = 0;
    std::unique_ptr<VertexBuffer> m_vertex_buffer;
    std::unique_ptr<IndexBuffer> m_index_buffer;
};

}
