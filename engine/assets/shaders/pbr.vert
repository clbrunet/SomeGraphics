#version 450 core

const uint MAX_LIGHTS_COUNT = 32;
const uint MAX_BONES_COUNT = 256;

struct Light {
    vec3 position;
    vec3 hdr_color;
};

layout(std140, binding = 0) uniform Globals {
    mat4 u_view_projection;
    vec3 u_camera_position;
    uint u_lights_count;
    Light u_lights[MAX_LIGHTS_COUNT];
    uint u_shadow_maps_count;
};

layout(std140, binding = 1) uniform MeshInfo {
    mat4 u_model;
    bool u_is_rigged;
    mat4 u_bone_transforms[MAX_BONES_COUNT];
};

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_texture_coordinates;
layout(location = 4) in uint a_bone_indices;
layout(location = 5) in vec3 a_weights;

out vec3 v_position;
out mat3 v_TBN;
out vec2 v_texture_coordinates;

void main()
{
    vec4 local_position;
    vec3 local_normal;
    vec3 local_tangent;
    if (u_is_rigged) {
        const uint bone_index_mask = 0xFF;
        mat4 weights_transform = u_bone_transforms[a_bone_indices >> 24] * a_weights[0]
            + u_bone_transforms[(a_bone_indices >> 16) & bone_index_mask] * a_weights[1]
            + u_bone_transforms[(a_bone_indices >> 8) & bone_index_mask] * a_weights[2]
            + u_bone_transforms[a_bone_indices & bone_index_mask]
            * (1.0 - (a_weights[0] + a_weights[1] + a_weights[2]));
        local_position = weights_transform * a_position;
        local_normal = mat3(weights_transform) * a_normal;
        local_tangent = mat3(weights_transform) * a_tangent;
    } else {
        local_position = a_position;
        local_normal = a_normal;
        local_tangent = a_tangent;
    }
    vec4 position = u_model * local_position;
    v_position = vec3(position);
    vec3 N = normalize(vec3(u_model * vec4(local_normal, 0.0)));
    vec3 T = normalize(vec3(u_model * vec4(local_tangent, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    v_TBN = mat3(T, B, N);
    v_texture_coordinates = a_texture_coordinates;
    gl_Position = u_view_projection * position;
}
