#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/vector_float3.hpp"

namespace sg {

class Camera {
public:
    Camera() = delete;
    Camera(const glm::vec3& position, const glm::quat& rotation, const glm::mat4& projection);
    Camera(Camera&& other) = default;
    Camera(const Camera& other) = default;
    Camera& operator=(Camera&& other) = default;
    Camera& operator=(const Camera& other) = default;
    virtual ~Camera();

    const glm::mat4& view_projection() const;
    void set_position(const glm::vec3& position);
    void set_rotation(const glm::quat& rotation);
    void set_projection(const glm::mat4& projection);
protected:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::mat4 m_projection;
    glm::mat4 m_view_projection;
private:
    void set_view_projection();
};

}
