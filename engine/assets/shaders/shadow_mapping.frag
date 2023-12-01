#version 450 core

const float light_far = 30.0;

uniform vec3 u_light_position;

in vec3 v_position;

void main()
{
    float distance = length(v_position - u_light_position);
    gl_FragDepth = distance / light_far;
}
