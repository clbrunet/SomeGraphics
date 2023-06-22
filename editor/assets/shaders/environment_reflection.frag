#version 450 core

in vec3 v_position;
in vec3 v_normal;

uniform vec3 u_camera_position;
uniform samplerCube u_environment;

out vec4 color;

void main()
{
    vec3 camera_to_fragment = v_position - u_camera_position;
    vec3 fragment_to_environment = reflect(normalize(camera_to_fragment), v_normal);
    color = texture(u_environment, fragment_to_environment);
}
