#version 450 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texture_coordinates;

out vec2 v_texture_coordinates;

void main()
{
    v_texture_coordinates = a_texture_coordinates;
    gl_Position = vec4(a_position, 0.0, 1.0);
}
