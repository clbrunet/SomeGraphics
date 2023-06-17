#include "SomeGraphics/Rendering/VertexAttribute.hpp"

namespace sg {

VertexAttribute::VertexAttribute(VertexAttributeType type)
{
    switch (type) {
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
        case VertexAttributeType::Float:
        case VertexAttributeType::Vec2:
        case VertexAttributeType::Vec3:
            m_type = GL_FLOAT;
            m_size = m_count * sizeof(float);
            break;
    }
}

VertexAttribute::~VertexAttribute()
{
}

int VertexAttribute::count() const
{
    return m_count;
}

int VertexAttribute::type() const
{
    return m_type;
}

int VertexAttribute::size() const
{
    return m_size;
}

}
