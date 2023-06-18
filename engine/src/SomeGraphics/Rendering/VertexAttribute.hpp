#pragma once

#include <concepts>

#include "glad/gl.h"

namespace sg {

enum class VertexAttributeType {
    Float,
    Vec2,
    Vec3,
};

class VertexAttribute {
public:
    VertexAttribute() = delete;
    VertexAttribute(VertexAttributeType type);
    VertexAttribute(VertexAttribute&& other) = default;
    VertexAttribute(const VertexAttribute& other) = default;
    VertexAttribute& operator=(VertexAttribute&& other) = default;
    VertexAttribute& operator=(const VertexAttribute& other) = default;
    ~VertexAttribute();

    int count() const;
    int size() const;
    int type() const;
private:
    int m_count;
    GLenum m_type;
    int m_size;
};

}
