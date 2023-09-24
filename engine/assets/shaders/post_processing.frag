#version 450 core

uniform sampler2D u_texture;

in vec2 v_texture_coordinates;

out vec4 color;

void main()
{
    vec3 pixel = texture(u_texture, v_texture_coordinates).rgb;
    // tone mapping
    pixel = pixel / (pixel + 1.0);
    // gamma correction
    float gamma = 2.2;
    pixel = pow(pixel, vec3(1.0 / gamma));
    color = vec4(pixel, 1.0);
}
