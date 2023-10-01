#include <cassert>

#include <GLFW/glfw3.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Editor/EditorCamera.hpp"

namespace sg {

EditorCamera::EditorCamera(glm::vec3 position, const glm::vec2& euler_angles,
    glm::mat4 projection) :
    Camera(std::move(position), glm::quat(glm::radians(glm::vec3(euler_angles, 0.0f))),
        std::move(projection)),
    m_pitch(euler_angles.x),
    m_yaw(euler_angles.y)
{
    assert(-90.0f <= euler_angles.x && euler_angles.x <= 90.0f);
}

void EditorCamera::on_update(const Window& window)
{
    look(window);
    move(window);
}

void EditorCamera::look(const Window& window)
{
    glm::vec2 change = -SENSITIVITY * window.cursor_delta();
    m_pitch = glm::clamp(m_pitch + change.y, -90.0f, 90.0f);
    m_yaw += change.x;
    set_rotation(glm::quat(glm::radians(glm::vec3(m_pitch, m_yaw, 0.0f))));
}

void EditorCamera::move(const Window& window)
{
    glm::vec3 direction = glm::vec3(0.0f);
    if (window.is_key_pressed(GLFW_KEY_W)) {
        direction.z -= 1.0f;
    }
    if (window.is_key_pressed(GLFW_KEY_S)) {
        direction.z += 1.0f;
    }
    if (window.is_key_pressed(GLFW_KEY_A)) {
        direction.x -= 1.0f;
    }
    if (window.is_key_pressed(GLFW_KEY_D)) {
        direction.x += 1.0f;
    }
    if (window.is_key_pressed(GLFW_KEY_Q)) {
        direction.y -= 1.0f;
    }
    if (window.is_key_pressed(GLFW_KEY_E)) {
        direction.y += 1.0f;
    }
    if (direction == glm::vec3(0.0f)) {
        return;
    }
    set_position(m_position + window.delta_time()
        * SPEED * (m_rotation * glm::normalize(direction)));
}

}
