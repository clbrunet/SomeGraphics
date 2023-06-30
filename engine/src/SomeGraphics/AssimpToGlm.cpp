#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"

#include "SomeGraphics/AssimpToGlm.hpp"

namespace sg {

glm::vec3 AssimpToGlm::vec3(const aiVector3D& vec3)
{
    return glm::vec3(vec3.x, vec3.y, vec3.z);
}

glm::vec3 AssimpToGlm::vec3(const aiColor3D& color3)
{
    return glm::vec3(color3.r, color3.g, color3.b);
}

glm::mat4 AssimpToGlm::mat4(const aiMatrix4x4& mat4)
{
    return glm::mat4(mat4.a1, mat4.b1, mat4.c1, mat4.d1, mat4.a2, mat4.b2, mat4.c2, mat4.d2,
        mat4.a3, mat4.b3, mat4.c3, mat4.d3, mat4.a4, mat4.b4, mat4.c4, mat4.d4);
}

}
