#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

namespace sg {

class Camera {
public:
    Camera(const glm::vec3& position, const glm::vec3& direction, const glm::mat4& projection);
    ~Camera();

    const glm::mat4& view_projection() const;
private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::mat4 m_view_projection;
    glm::mat4 m_projection;
};

}
