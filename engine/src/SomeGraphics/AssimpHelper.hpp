#pragma once

#include <assimp/types.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace sg::assimp_helper {

glm::vec3 vec3(const aiVector3D& vec3);

glm::vec3 vec3(const aiColor3D& color3);

glm::mat4 mat4(const aiMatrix4x4& mat4);

glm::quat quat(const aiQuaternion& quat);

bool operator==(const aiString& ai_string, const std::string& string);

}
