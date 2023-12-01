#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/vector_float3.hpp>

namespace sg {

class Camera {
public:
    Camera() = delete;
    Camera(glm::vec3 position, glm::quat rotation, glm::mat4 projection);
    Camera(Camera&& other) = default;
    Camera(const Camera& other) = default;
    Camera& operator=(Camera&& other) = default;
    Camera& operator=(const Camera& other) = default;
    ~Camera() = default;

    const glm::vec3& position() const;
    const glm::quat& rotation() const;
    const glm::mat4& view() const;
    const glm::mat4& projection() const;
    const glm::mat4& view_projection() const;
    void set_position(glm::vec3 position);
    void set_rotation(glm::quat rotation);
    void set_projection(glm::mat4 projection);
protected:
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    glm::mat4 m_view_projection;
private:
    void set_view();
    void set_view_projection();
};

}
