#version 450 core

layout (location = 0) in vec4 a_position;
layout (location = 1) in vec3 a_color;

uniform mat4 u_view_projection;
uniform mat4 u_model;

out vec3 v_color;

void main()
{
    v_color = a_color;
    gl_Position = u_view_projection * u_model * a_position;
}
