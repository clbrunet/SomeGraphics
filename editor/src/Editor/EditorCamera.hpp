#pragma once

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float2.hpp>
#include <SomeGraphics.hpp>

namespace sg {

class EditorCamera {
public:
    EditorCamera() = delete;
    EditorCamera(glm::vec3 position, const glm::vec2& euler_angles, glm::mat4 projection);
    EditorCamera(EditorCamera&& other) = default;
    EditorCamera(const EditorCamera& other) = default;
    EditorCamera& operator=(EditorCamera&& other) = default;
    EditorCamera& operator=(const EditorCamera& other) = default;
    ~EditorCamera() = default;

    Camera camera;

    void update(const Window& window);
private:
    static constexpr float SENSITIVITY = 0.2f;
    static constexpr float BASE_SPEED = 5.0f;
    float m_pitch = 0.0f;
    float m_yaw = 0.0f;

    void look(const Window& window);
    void move(const Window& window);
};

}
