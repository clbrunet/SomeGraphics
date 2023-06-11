#pragma once

#include <sys/types.h>
#include <vector>
#include <initializer_list>

#include "glad/gl.h"

#include "SomeGraphics/VertexAttribute.hpp"

namespace sg {

class VertexBuffer {
public:
    template<typename T>
    VertexBuffer(const std::vector<T>& vertices, std::initializer_list<VertexAttribute> attributes)
    {
        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
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
    ~VertexBuffer();
private:
    uint m_id = 0;
};

}
