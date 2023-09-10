#include <cassert>

#include "GLFW/glfw3.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Editor/EditorCamera.hpp"

namespace sg {

EditorCamera::EditorCamera(glm::vec3 position, const glm::vec2& euler_angles,
    glm::mat4 projection) :
    Camera(std::move(position), glm::quat(glm::radians(glm::vec3(euler_angles, 0.0f))),
        std::move(projection)),
    m_pitch(euler_angles.x),
    m_yaw(euler_angles.y)
{
    assert(-89.0f <= euler_angles.x && euler_angles.x <= 89.0f);
}

void EditorCamera::on_update(const Window& window, float delta_time)
{
    if (!window.is_mouse_button_pressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        m_is_free_flying = false;
        return;
    }
    if (!m_is_free_flying) {
        m_is_free_flying = true;
        m_last_cursor_position = window.get_cursor_position();
        return;
    }
    look(window);
    move(window, delta_time);
}

void EditorCamera::look(const Window& window)
{
    glm::vec2 cursor_position = window.get_cursor_position();
    glm::vec2 change = -SENSITIVITY * (cursor_position - m_last_cursor_position);
    m_last_cursor_position = cursor_position;
    m_pitch = glm::clamp(m_pitch + change.y, -89.0f, 89.0f);
    m_yaw += change.x;
    set_rotation(glm::quat(glm::radians(glm::vec3(m_pitch, m_yaw, 0.0f))));
}

void EditorCamera::move(const Window& window, float delta_time)
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
    set_position(m_position + delta_time * SPEED * (m_rotation * glm::normalize(direction)));
}

}
