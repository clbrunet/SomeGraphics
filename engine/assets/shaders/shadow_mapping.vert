#version 450 core

uniform mat4 u_view_projection;
uniform mat4 u_model;

layout(location = 0) in vec4 a_position;

out vec3 v_position;

void main()
{
    vec4 position = u_model * a_position;
    v_position = vec3(position);
    gl_Position = u_view_projection * position;
}
