#version 450 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texture_coordinates;

uniform vec3 u_camera_position;
uniform samplerCube u_environment;
uniform sampler2D u_texture;
uniform vec3 u_color;

out vec4 color;

void main()
{
    vec3 camera_to_fragment = v_position - u_camera_position;
    vec3 fragment_to_environment = reflect(normalize(camera_to_fragment), v_normal);
    color = mix(texture(u_environment, fragment_to_environment),
        texture(u_texture, v_texture_coordinates) * vec4(u_color, 1.0), 0.7);
}
