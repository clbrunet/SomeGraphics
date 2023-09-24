#version 450 core

uniform sampler2D u_texture;

in vec2 v_texture_coordinates;

out vec4 color;

vec3 narkowicz_aces_tone_mapping(vec3 x)
{
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

void main()
{
    vec3 pixel = texture(u_texture, v_texture_coordinates).rgb;
    pixel = narkowicz_aces_tone_mapping(pixel);
    float gamma = 2.2;
    pixel = pow(pixel, vec3(1.0 / gamma));
    color = vec4(pixel, 1.0);
}
