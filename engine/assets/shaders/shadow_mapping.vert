#version 450 core

const uint MAX_BONES_COUNT = 256;

layout(std140, binding = 1) uniform MeshInfo {
    mat4 u_model;
    bool u_is_rigged;
    mat4 u_bone_transforms[MAX_BONES_COUNT];
};

uniform mat4 u_view_projection;

layout(location = 0) in vec4 a_position;
layout(location = 4) in uint a_bone_indices;
layout(location = 5) in vec3 a_weights;

out vec3 v_position;

void main()
{
    vec4 local_position;
    if (u_is_rigged) {
        const uint bone_index_mask = 0xFF;
        mat4 weights_transform = u_bone_transforms[a_bone_indices >> 24] * a_weights[0]
            + u_bone_transforms[(a_bone_indices >> 16) & bone_index_mask] * a_weights[1]
            + u_bone_transforms[(a_bone_indices >> 8) & bone_index_mask] * a_weights[2]
            + u_bone_transforms[a_bone_indices & bone_index_mask]
            * (1.0 - (a_weights[0] + a_weights[1] + a_weights[2]));
        local_position = weights_transform * a_position;
    }
    else {
        local_position = a_position;
    }
    vec4 position = u_model * local_position;
    v_position = vec3(position);
    gl_Position = u_view_projection * position;
}
