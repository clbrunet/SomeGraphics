#version 450 core

in vec2 v_texture_coordinates;

uniform sampler2D u_texture;

out vec4 color;

void main()
{
    color = texture(u_texture, v_texture_coordinates);
    float gamma = 2.2;
    color.rgb = pow(color.rgb, vec3(1.0 / gamma));
}
