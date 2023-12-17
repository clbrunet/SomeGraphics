#pragma once

#include <cstdint>
#include <iostream>
#include <sys/types.h>
#include <type_traits>
#include <span>

#include <glad/gl.h>
#include <glm/gtx/string_cast.hpp>

namespace sg {

class VertexBuffer {
public:
    VertexBuffer() = delete;
    template<typename T>
    VertexBuffer(std::span<const T> vertices)
    {
        glCreateBuffers(1, &m_renderer_id);
        glNamedBufferStorage(m_renderer_id, vertices.size() * sizeof(T), vertices.data(), 0);
    }
    VertexBuffer(VertexBuffer&& other);
    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(VertexBuffer&& other);
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    ~VertexBuffer();

    void bind_to_vertex_array(uint32_t vertex_array, uint32_t binding_index,
        intptr_t offset, uint32_t stride) const;
private:
    uint32_t m_renderer_id = 0;
};

}
