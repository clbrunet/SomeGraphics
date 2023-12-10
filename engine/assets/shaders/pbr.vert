#version 450 core

const uint MAX_LIGHTS_COUNT = 32;

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

uniform mat4 u_model;

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec2 a_texture_coordinates;

out vec3 v_position;
out mat3 v_TBN;
out vec2 v_texture_coordinates;

void main()
{
    vec4 position = u_model * a_position;
    v_position = vec3(position);
    vec3 N = normalize(vec3(u_model * vec4(a_normal, 0.0)));
    vec3 T = normalize(vec3(u_model * vec4(a_tangent, 0.0)));
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    v_TBN = mat3(T, B, N);
    v_texture_coordinates = a_texture_coordinates;
    gl_Position = u_view_projection * position;
}
