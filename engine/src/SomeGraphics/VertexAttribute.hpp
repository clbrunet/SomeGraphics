#pragma once

#include <concepts>
#include <glad/gl.h>

namespace sg {

enum class VertexAttributeType {
    Float,
};

class VertexAttribute {
public:
    VertexAttribute(VertexAttributeType type);
    ~VertexAttribute();

    int size() const;
    int type() const;
private:
    int m_size;
    GLenum m_type;
};

}
