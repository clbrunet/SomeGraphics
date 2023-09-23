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
};

uniform mat4 u_model;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texture_coordinates;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texture_coordinates;

void main()
{
    v_position = vec3(u_model * vec4(a_position, 1.0));
    v_normal = normalize(transpose(inverse(mat3(u_model))) * a_normal);
    v_texture_coordinates = a_texture_coordinates;
    gl_Position = u_view_projection * vec4(v_position, 1.0);
}
