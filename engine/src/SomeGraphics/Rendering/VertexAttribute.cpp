#include "SomeGraphics/Rendering/VertexAttribute.hpp"

namespace sg {

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

bool VertexAttribute::is_integer() const
{
    return m_type == GL_UNSIGNED_INT;
}

}
