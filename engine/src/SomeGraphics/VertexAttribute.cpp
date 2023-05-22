#include "SomeGraphics/VertexAttribute.hpp"

namespace sg {

VertexAttribute::VertexAttribute(VertexAttributeType type)
{
    switch (type) {
        case VertexAttributeType::Float:
            m_size = 1;
            m_type = GL_FLOAT;
            break;
    }
}

VertexAttribute::~VertexAttribute()
{
}

int VertexAttribute::size() const
{
    return m_size;
}

int VertexAttribute::type() const
{
    return m_type;
}

}
