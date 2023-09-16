#pragma once

#include <glm/ext/matrix_float4x4.hpp>

namespace sg {

struct Transform {
    Transform() = default;
    Transform(const glm::mat4& mat4);
    Transform(Transform&& other) = default;
    Transform(const Transform& other) = default;
    Transform& operator=(Transform&& other) = default;
    Transform& operator=(const Transform& other) = default;
    ~Transform() = default;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 compute_matrix() const;
};

}
