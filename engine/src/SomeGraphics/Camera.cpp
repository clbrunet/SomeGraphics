#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/matrix.hpp"

#include "SomeGraphics/Camera.hpp"

namespace sg {

Camera::Camera(const glm::vec3& position, const glm::quat& rotation, const glm::mat4& projection) :
    m_position(position),
    m_rotation(rotation),
    m_projection(projection)
{
    set_view_projection();
}

Camera::~Camera()
{
}

const glm::mat4& Camera::view_projection() const
{
    return m_view_projection;
}

void Camera::set_position(const glm::vec3& position)
{
    m_position = position;
    set_view_projection();
}

void Camera::set_rotation(const glm::quat& rotation)
{
    m_rotation = rotation;
    set_view_projection();
}

void Camera::set_view_projection()
{
    m_view_projection = m_projection
        * glm::inverse(glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_rotation));
}

}
