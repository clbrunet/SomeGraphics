#version 450 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;

uniform mat4 u_view_projection;
uniform mat4 u_model;

out vec3 v_position;
out vec3 v_normal;

void main()
{
    v_position = vec3(u_model * vec4(a_position, 1.0));
    v_normal = normalize(transpose(inverse(mat3(u_model))) * a_normal);
    gl_Position = u_view_projection * vec4(v_position, 1.0);
}
