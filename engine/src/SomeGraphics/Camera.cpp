#include <iostream>

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

#include "SomeGraphics/Camera.hpp"

namespace sg {

Camera::Camera(const glm::vec3& position, const glm::vec3& direction, const glm::mat4& projection) :
    m_position(position),
    m_direction(direction),
    m_view_projection(projection
        * glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f))),
    m_projection(projection)
{
    std::cout << glm::to_string(m_view_projection) << std::endl;
}

Camera::~Camera()
{
}

const glm::mat4& Camera::view_projection() const
{
    return m_view_projection;
}

}
