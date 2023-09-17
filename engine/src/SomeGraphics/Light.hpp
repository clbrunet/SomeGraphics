#pragma once

#include <glm/ext/vector_float3.hpp>

namespace sg {

struct Light {
    Light() = default;
    Light(glm::vec3 color);
    Light(glm::vec3 color, float intensity);
    Light(Light&& other) = default;
    Light(const Light& other) = default;
    Light& operator=(Light&& other) = default;
    Light& operator=(const Light& other) = default;
    ~Light() = default;

    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;
};

}
