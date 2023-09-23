#version 450 core

uniform mat4 u_view;
uniform mat4 u_projection;

layout(location = 0) in vec4 a_position;

out vec3 v_texture_coordinates;

void main()
{
    v_texture_coordinates = vec3(a_position);
    gl_Position = ((u_projection * mat4(mat3(u_view))) * a_position).xyww;
}
