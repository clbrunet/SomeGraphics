#version 450 core

layout (location = 0) in vec4 position;

uniform mat4 view_projection;

void main()
{
    gl_Position = view_projection * position;
}
