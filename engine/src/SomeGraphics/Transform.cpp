#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "glm/gtx/quaternion.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

#include "SomeGraphics/Transform.hpp"

namespace sg {

Transform::Transform(const glm::mat4& mat4)
{
    glm::quat orientation;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(mat4, scale, orientation, position, skew, perspective);
    rotation = glm::eulerAngles(orientation);
}

glm::mat4 Transform::compute_matrix() const
{
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
    matrix *= glm::toMat4(glm::quat(rotation));
    return glm::scale(matrix, scale);
}

}
