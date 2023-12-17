#pragma once

#include <sys/types.h>
#include <span>

#include <glad/gl.h>

namespace sg {

class IndexBuffer {
public:
    IndexBuffer() = delete;
    IndexBuffer(std::span<const uint32_t> indices);
    IndexBuffer(IndexBuffer&& other);
    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other);
    IndexBuffer& operator=(const IndexBuffer& other) = delete;
    ~IndexBuffer();

    uint32_t count() const;
    GLenum format() const;
    void bind_to_vertex_array(uint32_t vertex_array) const;
private:
    uint32_t m_renderer_id = 0;
    uint32_t m_count = 0;
    GLenum m_format = GL_UNSIGNED_INT;
};

}
