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

    constexpr VertexAttribute(VertexAttributeType type) :
        m_count(count(type)),
        m_type(VertexAttribute::type(type)),
        m_size(size(type, m_count))
    {
    }

    constexpr VertexAttribute(VertexAttribute&& other) = default;
    constexpr VertexAttribute(const VertexAttribute& other) = default;
    constexpr VertexAttribute& operator=(VertexAttribute&& other) = default;
    constexpr VertexAttribute& operator=(const VertexAttribute& other) = default;
    constexpr ~VertexAttribute() = default;


    constexpr uint32_t count() const
    {
        return m_count;
    }

    constexpr static uint32_t count(VertexAttributeType type)
    {
        switch (type) {
        case VertexAttributeType::Uint:
        case VertexAttributeType::Float:
            return 1;
        case VertexAttributeType::Vec2:
            return 2;
        case VertexAttributeType::Vec3:
            return 3;
        }
        return 0;
    }

    constexpr uint32_t type() const
    {
        return m_type;
    }

    constexpr static GLenum type(VertexAttributeType type)
    {

        switch (type) {
        case VertexAttributeType::Uint:
            return GL_UNSIGNED_INT;
        case VertexAttributeType::Float:
        case VertexAttributeType::Vec2:
        case VertexAttributeType::Vec3:
            return GL_FLOAT;
        }
        return 0;
    }

    constexpr uint32_t size() const
    {
        return m_size;
    }

    constexpr static uint32_t size(VertexAttributeType type, uint32_t count)
    {
        switch (type) {
        case VertexAttributeType::Uint:
            return count * sizeof(uint32_t);
        case VertexAttributeType::Float:
        case VertexAttributeType::Vec2:
        case VertexAttributeType::Vec3:
            return count * sizeof(float);
        }
        return 0;
    }

    constexpr bool is_integer() const
    {
        return m_type == GL_UNSIGNED_INT;
    }

private:
    uint32_t m_count;
    GLenum m_type;
    uint32_t m_size;
};

}
