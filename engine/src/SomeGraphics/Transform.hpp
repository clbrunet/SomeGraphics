#pragma once

#include "glm/ext/matrix_float4x4.hpp"

namespace sg {

class Transform {
public:
    Transform() = default;
    Transform(glm::mat4 local);
    Transform(Transform&& other) = default;
    Transform(const Transform& other) = default;
    Transform& operator=(Transform&& other) = default;
    Transform& operator=(const Transform& other) = default;
    ~Transform() = default;

    const glm::mat4& local() const;
    const glm::vec3& local_position() const;
    const glm::vec3& local_rotation() const;
    const glm::vec3& local_scale() const;

    void set_local_position(glm::vec3 local_position);
    void set_local_rotation(glm::vec3 local_rotation);
    void set_local_scale(glm::vec3 local_scale);
private:
    glm::mat4 m_local = glm::mat4(1.0f);
    glm::vec3 m_local_position = glm::vec3(0.0f);
    glm::vec3 m_local_rotation = glm::vec3(0.0f);
    glm::vec3 m_local_scale = glm::vec3(1.0f);

    void set_local();
};

}
