#pragma once

#include <assimp/matrix4x4.h>
#include <assimp/types.h>
#include <assimp/vector3.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

namespace sg {

class AssimpToGlm {
public:
    AssimpToGlm() = delete;
    AssimpToGlm(AssimpToGlm&& other) = delete;
    AssimpToGlm(const AssimpToGlm& other) = delete;
    AssimpToGlm& operator=(AssimpToGlm&& other) = delete;
    AssimpToGlm& operator=(const AssimpToGlm& other) = delete;
    ~AssimpToGlm() = delete;

    static glm::vec3 vec3(const aiVector3D& vec3);
    static glm::vec3 vec3(const aiColor3D& color3);
    static glm::mat4 mat4(const aiMatrix4x4& mat4);
};

}
