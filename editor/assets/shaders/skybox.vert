#version 450 core

layout (location = 0) in vec4 a_position;

uniform mat4 u_view_projection;

out vec3 v_texture_coordinates;

void main()
{
    v_texture_coordinates = vec3(a_position);
    gl_Position = (u_view_projection * a_position).xyww;
}
