#version 450 core

in vec3 v_texture_coordinates;

uniform samplerCube skybox;

out vec4 color;

void main()
{
    color = texture(skybox, v_texture_coordinates);
}
