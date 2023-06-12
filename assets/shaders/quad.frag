#version 450 core

in vec2 v_texture_coordinates;

uniform sampler2D u_texture;

out vec4 color;

void main()
{
    color = vec4(1.0 - vec3(texture(u_texture, v_texture_coordinates)), 1.0);
}
