#version 450 core

const float PI = 3.14159265359;

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texture_coordinates;

uniform vec3 u_camera_position;

uniform vec4 u_color;
uniform sampler2D u_albedo_map;
uniform sampler2D u_roughness_map;
uniform sampler2D u_metalness_map;

out vec4 color;

vec3 cook_torrance_brdf(vec3 albedo, float roughness, float metalness, vec3 normal,
        vec3 fragment_position, vec3 camera_position, vec3 light_position);

void main()
{
    vec4 albedo = texture(u_albedo_map, v_texture_coordinates) * u_color;
    float roughness = texture(u_roughness_map, v_texture_coordinates).g;
    float metalness = texture(u_metalness_map, v_texture_coordinates).b;
    vec3 light_position = vec3(0.0, 1.0, 2.0);
    vec3 light_color = vec3(3.0);
    vec3 brdf = cook_torrance_brdf(albedo.rgb, roughness, metalness,
            v_normal, v_position, u_camera_position, light_position);
    vec3 fragment_to_light = light_position - v_position;
    float fragment_to_light_distance = length(fragment_to_light);
    float attenuation = 1.0 / (fragment_to_light_distance * fragment_to_light_distance);
    vec3 radiance = light_color * attenuation;
    float normal_dot_fragment_to_light = max(dot(v_normal, fragment_to_light), 0.0);
    vec3 ambient = vec3(0.03) * albedo.rgb;
    color = vec4(brdf * radiance * normal_dot_fragment_to_light + ambient, albedo.a);
}

vec3 lambertian_diffuse(vec3 albedo)
{
    return albedo / PI;
}

float trowbridge_reitz_ggx_normal_distribution_function(float roughness,
        vec3 normal, vec3 halfway_vector)
{
    float alpha = pow(roughness, 4.0);
    return alpha
        / (PI * pow(pow(max(dot(normal, halfway_vector), 0.0), 2.0)
                    * (alpha - 1.0) + 1.0, 2.0));
}

float schlick_ggx_geometry_function(vec3 normal, vec3 from_fragment_vector,
        float reflection_coefficient)
{
    float normal_dot_from_fragment_vector = max(dot(normal, from_fragment_vector), 0.0);
    return normal_dot_from_fragment_vector
        / (normal_dot_from_fragment_vector * (1.0 - reflection_coefficient) + reflection_coefficient);
}

float smith_schlick_ggx_geometry_function(float roughness, vec3 normal,
        vec3 fragment_to_camera, vec3 fragment_to_light)
{
    float reflection_coefficient = pow(roughness + 1.0, 2.0) / 8.0;
    return schlick_ggx_geometry_function(normal, fragment_to_camera, reflection_coefficient)
        * schlick_ggx_geometry_function(normal, fragment_to_light, reflection_coefficient);
}

vec3 fresnel_schlick_fresnel_equation(vec3 albedo, float metalness,
        vec3 halfway_vector, vec3 fragment_to_camera)
{
    vec3 base_reflection_coefficient = mix(vec3(0.04), albedo, metalness);
    return base_reflection_coefficient + (1.0 - base_reflection_coefficient)
        * pow(1.0 - max(dot(halfway_vector, fragment_to_camera), 0.0), 5.0);
}

vec3 cook_torrance_brdf(vec3 albedo, float roughness, float metalness, vec3 normal,
        vec3 fragment_position, vec3 camera_position, vec3 light_position)
{
    vec3 fragment_to_camera = normalize(camera_position - fragment_position);
    vec3 fragment_to_light = normalize(light_position - fragment_position);
    vec3 halfway_vector = normalize(fragment_to_camera + fragment_to_light);

    float normal_distribution = trowbridge_reitz_ggx_normal_distribution_function(roughness,
            normal, halfway_vector);
    float microfacet_shadowing =  smith_schlick_ggx_geometry_function(roughness, normal,
            fragment_to_camera, fragment_to_light);
    vec3 reflection_ratio = fresnel_schlick_fresnel_equation(albedo, metalness,
            halfway_vector, fragment_to_camera);
    vec3 refraction_ratio = (vec3(1.0) - reflection_ratio) * (1.0 - metalness);

    vec3 refraction = refraction_ratio * lambertian_diffuse(albedo);
    vec3 reflection = (normal_distribution * microfacet_shadowing * reflection_ratio)
        / (4.0 * max(dot(fragment_to_camera, normal), 0.0)
                * max(dot(fragment_to_light, normal), 0.0) + 0.0001);
    return refraction + reflection;
}