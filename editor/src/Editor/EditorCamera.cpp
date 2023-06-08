#include <iostream>

#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/constants.hpp"

#include "Editor/EditorCamera.hpp"

namespace sg {

EditorCamera::EditorCamera(const glm::vec3& position, const glm::quat& rotation,
    const glm::mat4& projection) :
    Camera(position, rotation, projection)
{
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::on_update(const Window& window, float delta_time)
{
    glm::vec3 direction = glm::vec3(0.0f);
    if (window.get_key(GLFW_KEY_W)) {
        direction.z -= 1.0f;
    }
    if (window.get_key(GLFW_KEY_S)) {
        direction.z += 1.0f;
    }
    if (window.get_key(GLFW_KEY_A)) {
        direction.x -= 1.0f;
    }
    if (window.get_key(GLFW_KEY_D)) {
        direction.x += 1.0f;
    }
    if (direction == glm::vec3(0.0f)) {
        return;
    }
    direction = glm::normalize(direction);
    set_position(m_position + m_rotation * (delta_time * SPEED * direction));
}

}
