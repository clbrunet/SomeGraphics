#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 color;

uniform mat4 view_projection;

out vec3 v_color;

void main()
{
    v_color = color;
    gl_Position = view_projection * position;
}
