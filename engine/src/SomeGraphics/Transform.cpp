#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "SomeGraphics/Transform.hpp"

namespace sg {

Transform::Transform(const glm::mat4& local) :
    m_local(local)
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(local, scale, rotation, translation, skew, perspective);
    m_local_position = translation;
}

const glm::mat4& Transform::local() const
{
    return m_local;
}

const glm::vec3& Transform::local_position() const
{
    return m_local_position;
}

void Transform::set_local_position(const glm::vec3& local_position)
{
    glm::vec3 offset = local_position - m_local_position;
    m_local_position = local_position;
    m_local = glm::translate(m_local, offset);
}

}
