#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float2.hpp"

#include "SomeGraphics.hpp"

namespace sg {

class EditorCamera final : public Camera {
public:
    EditorCamera(const glm::vec3& position, const glm::vec2& euler_angles,
        const glm::mat4& projection);
    ~EditorCamera();

    void on_update(const Window& window, float delta_time);
private:
    static constexpr float SENSITIVITY = 0.5f;
    static constexpr float SPEED = 5.0f;
    float m_pitch = 0.0f;
    float m_yaw = 0.0f;
    bool m_is_free_flying = false;
    glm::vec2 m_last_cursor_position = glm::vec2(0.0f);

    void look(const Window& window);
    void move(const Window& window, float delta_time);
};

}
