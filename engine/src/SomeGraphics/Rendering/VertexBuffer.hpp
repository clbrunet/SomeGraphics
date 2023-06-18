#pragma once

#include <sys/types.h>
#include <vector>
#include <initializer_list>

#include "glad/gl.h"

#include "SomeGraphics/Rendering/VertexAttribute.hpp"

namespace sg {

class VertexBuffer {
public:
    VertexBuffer() = delete;
    template<typename T>
    VertexBuffer(const std::vector<T>& vertices, std::initializer_list<VertexAttribute> attributes)
    {
        glCreateBuffers(1, &m_renderer_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);

        uint stride = 0;
        for (const VertexAttribute& attribute : attributes) {
            stride += attribute.size();
        }
        uint index = 0;
        uint64_t offset = 0;
        for (const VertexAttribute& attribute : attributes) {
            glVertexAttribPointer(index, attribute.count(), attribute.type(), GL_FALSE,
                stride, (const void*)offset);
            glEnableVertexAttribArray(index);
            index++;
            offset += attribute.size();
        }
    }
    VertexBuffer(VertexBuffer&& other);
    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(VertexBuffer&& other);
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    ~VertexBuffer();
private:
    uint m_renderer_id = 0;
};

}
