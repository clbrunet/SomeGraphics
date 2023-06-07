#include "glm/ext/quaternion_float.hpp"
#include "SomeGraphics.hpp"

namespace sg {

class EditorCamera final : public Camera {
public:
    EditorCamera(const glm::vec3& position, const glm::quat& rotation, const glm::mat4& projection);
    ~EditorCamera();

    void on_update(const Window& window, float delta_time);
private:
    static constexpr float SPEED = 5.0f;
};

}
