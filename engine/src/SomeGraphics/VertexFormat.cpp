#include <sys/types.h>

#include "SomeGraphics/VertexFormat.hpp"
#include "SomeGraphics/VertexAttribute.hpp"

namespace sg {

VertexFormat::VertexFormat(std::initializer_list<VertexAttribute> attributes)
{
    uint index = 0;
    uint64_t offset = 0;
    for (const VertexAttribute& attribute : attributes) {
        glVertexAttribPointer(index, attribute.count(), attribute.type(), GL_FALSE,
            attribute.size(), (const void*)offset);
        glEnableVertexAttribArray(index);
        index++;
        offset += attribute.size();
    }
}

VertexFormat::~VertexFormat()
{
}

}
