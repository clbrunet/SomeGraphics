#include <cstring>

#include "SomeGraphics/AssimpHelper.hpp"

namespace sg::assimp_helper {

glm::vec3 vec3(const aiVector3D& vec3)
{
    return glm::vec3(vec3.x, vec3.y, vec3.z);
}

glm::vec3 vec3(const aiColor3D& color3)
{
    return glm::vec3(color3.r, color3.g, color3.b);
}

glm::mat4 mat4(const aiMatrix4x4& mat4)
{
    return glm::mat4(mat4.a1, mat4.b1, mat4.c1, mat4.d1, mat4.a2, mat4.b2, mat4.c2, mat4.d2,
        mat4.a3, mat4.b3, mat4.c3, mat4.d3, mat4.a4, mat4.b4, mat4.c4, mat4.d4);
}

glm::quat quat(const aiQuaternion& quat)
{
    return glm::quat(quat.w, quat.x, quat.y, quat.z);
}

bool operator==(const aiString& ai_string, const std::string& string)
{
    return ai_string.length == string.length()
        && std::memcmp(ai_string.C_Str(), string.c_str(), ai_string.length) == 0;
}

bool ai_node_has_bones(const aiNode& ai_node, const aiScene& ai_scene)
{
    for (uint32_t i = 0; i < ai_node.mNumMeshes; i++) {
        if (ai_scene.mMeshes[ai_node.mMeshes[i]]->HasBones()) {
            return true;
        }
    }
    return false;
}

}
