#version 450 core

uniform samplerCube u_skybox;

in vec3 v_texture_coordinates;

out vec4 color;

void main()
{
    color = texture(u_skybox, v_texture_coordinates);
}
