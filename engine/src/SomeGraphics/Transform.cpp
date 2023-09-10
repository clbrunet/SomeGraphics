#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtx/matrix_decompose.hpp"

#include "SomeGraphics/Transform.hpp"

namespace sg {

Transform::Transform(glm::mat4 local) :
    m_local(std::move(local))
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(m_local, scale, rotation, translation, skew, perspective);
    m_local_position = std::move(translation);
    m_local_rotation = std::move(glm::eulerAngles(rotation));
    m_local_scale = std::move(scale);
}

const glm::mat4& Transform::local() const
{
    return m_local;
}

const glm::vec3& Transform::local_position() const
{
    return m_local_position;
}

const glm::vec3& Transform::local_rotation() const
{
    return m_local_rotation;
}

const glm::vec3& Transform::local_scale() const
{
    return m_local_scale;
}

void Transform::set_local_position(glm::vec3 local_position)
{
    m_local_position = std::move(local_position);
    set_local();
}

void Transform::set_local_rotation(glm::vec3 local_rotation)
{
    m_local_rotation = std::move(local_rotation);
    set_local();
}

void Transform::set_local_scale(glm::vec3 local_scale)
{
    m_local_scale = std::move(local_scale);
    set_local();
}

void Transform::set_local()
{
    m_local = glm::translate(glm::mat4(1.0f), m_local_position);
    m_local *= glm::toMat4(glm::quat(m_local_rotation));
    m_local = glm::scale(m_local, m_local_scale);
}

}
