#pragma once

#include "glm/ext/matrix_float4x4.hpp"

namespace sg {

class Transform {
public:
    Transform() = default;
    Transform(const glm::mat4& local);
    Transform(Transform&& other) = default;
    Transform(const Transform& other) = default;
    Transform& operator=(Transform&& other) = default;
    Transform& operator=(const Transform& other) = default;
    ~Transform() = default;

    const glm::mat4& local() const;
    const glm::vec3& local_position() const;
    void set_local_position(const glm::vec3& local_position);
private:
    glm::mat4 m_local = glm::mat4(1.0f);
    glm::vec3 m_local_position = glm::vec3(0.0f);
};

}
