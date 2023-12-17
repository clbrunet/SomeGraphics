#pragma once

#include <concepts>

#include <glad/gl.h>

namespace sg {

enum class VertexAttributeType {
    Uint,
    Float,
    Vec2,
    Vec3,
};

class VertexAttribute {
public:
    VertexAttribute() = delete;
    constexpr VertexAttribute(VertexAttributeType type)
    {
        switch (type) {
            case VertexAttributeType::Uint:
            case VertexAttributeType::Float:
                m_count = 1;
                break;
            case VertexAttributeType::Vec2:
                m_count = 2;
                break;
            case VertexAttributeType::Vec3:
                m_count = 3;
                break;
        }
        switch (type) {
            case VertexAttributeType::Uint:
                m_type = GL_UNSIGNED_INT;
                m_size = m_count * sizeof(uint32_t);
                break;
            case VertexAttributeType::Float:
            case VertexAttributeType::Vec2:
            case VertexAttributeType::Vec3:
                m_type = GL_FLOAT;
                m_size = m_count * sizeof(float);
                break;
        }
    }

    VertexAttribute(VertexAttribute&& other) = default;
    VertexAttribute(const VertexAttribute& other) = default;
    VertexAttribute& operator=(VertexAttribute&& other) = default;
    VertexAttribute& operator=(const VertexAttribute& other) = default;
    ~VertexAttribute() = default;

    int count() const;
    int size() const;
    int type() const;
private:
    int m_count;
    GLenum m_type;
    int m_size;
};

}
