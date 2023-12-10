#version 450 core

const float PI = 3.14159265359;

const uint MAX_LIGHTS_COUNT = 32;
const uint MAX_SHADOW_MAPS_COUNT = 8;

struct Light {
    vec3 position;
    vec3 hdr_color;
};

layout(std140, binding = 0) uniform Globals {
    mat4 u_view_projection;
    vec3 u_camera_position;
    uint u_lights_count;
    Light u_lights[MAX_LIGHTS_COUNT];
    uint u_shadow_maps_count;
};

uniform samplerCube u_shadow_maps[MAX_SHADOW_MAPS_COUNT];

uniform vec4 u_color;
uniform float u_roughness;
uniform float u_metalness;
uniform bool u_use_roughness_map;
uniform bool u_use_metalness_map;
uniform bool u_use_normal_map;
uniform sampler2D u_albedo_map;
uniform sampler2D u_roughness_map;
uniform sampler2D u_metalness_map;
uniform sampler2D u_normal_map;

in vec3 v_position;
in mat3 v_TBN;
in vec2 v_texture_coordinates;

out vec4 color;

float get_shadow_factor(samplerCube shadow_map, vec3 light_to_frag,
    float light_to_frag_dist, float light_z_far, float bias, float camera_to_frag_dist);
vec3 cook_torrance_brdf(vec3 albedo, float roughness, float metalness, vec3 normal,
        vec3 frag_position, vec3 camera_position, vec3 light_position);

void main()
{
    vec4 albedo = texture(u_albedo_map, v_texture_coordinates) * u_color;
    float roughness;
    if (u_use_roughness_map) {
        roughness = texture(u_roughness_map, v_texture_coordinates).g;
    } else {
        roughness = u_roughness;
    }
    float metalness;
    if (u_use_metalness_map) {
        metalness = texture(u_metalness_map, v_texture_coordinates).b;
    } else {
        metalness = u_metalness;
    }
    vec3 normal;
    if (u_use_normal_map) {
        normal = texture(u_normal_map, v_texture_coordinates).xyz * 2.0 - 1.0;
    } else {
        normal = vec3(0, 0, 1);
    }
    normal = normalize(v_TBN * normal);
    vec3 irradiance = vec3(0.0);
    for (uint i = 0; i < u_lights_count; i++) {
        Light light = u_lights[i];
        vec3 frag_to_light = light.position - v_position;
        float frag_to_light_dist = length(frag_to_light);
        float shadow_factor = i >= u_shadow_maps_count ? 1.0 : get_shadow_factor(u_shadow_maps[i],
            -frag_to_light, frag_to_light_dist, 30.0, 0.05, length(u_camera_position - v_position));
        vec3 brdf = cook_torrance_brdf(albedo.rgb, roughness, metalness,
                normal, v_position, u_camera_position, light.position);
        float attenuation = 1.0 / (frag_to_light_dist * frag_to_light_dist);
        vec3 frag_to_light_dir = frag_to_light / frag_to_light_dist;
        float normal_dot_frag_to_light = max(dot(normal, frag_to_light_dir), 0.0);
        vec3 radiance = light.hdr_color * attenuation * normal_dot_frag_to_light;
        irradiance += shadow_factor * brdf * radiance;
    }
    vec3 ambient = vec3(0.075) * albedo.rgb;
    color = vec4(irradiance + ambient, albedo.a);
}

float get_shadow_factor(samplerCube shadow_map, vec3 light_to_frag,
    float light_to_frag_dist, float light_z_far, float bias, float camera_to_frag_dist)
{
    const uint SAMPLES_COUNT = 20;
    const vec3 sample_offset_dirs[SAMPLES_COUNT] = vec3[] (
        vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
        vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
        vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
        vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
        vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
    );
    float disk_radius = 0.01;
    float shadow = 0.0;
    for (int i = 0; i < SAMPLES_COUNT; i++) {
        float dist = texture(shadow_map, light_to_frag
            + disk_radius * sample_offset_dirs[i]).r * light_z_far;
        shadow += float(light_to_frag_dist > dist + bias);
    }
    return 1.0 - shadow / SAMPLES_COUNT;
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

float schlick_ggx_geometry_function(vec3 normal, vec3 from_frag_vector,
        float reflection_coefficient)
{
    float normal_dot_from_frag_vector = max(dot(normal, from_frag_vector), 0.0);
    return normal_dot_from_frag_vector
        / (normal_dot_from_frag_vector * (1.0 - reflection_coefficient) + reflection_coefficient);
}

float smith_schlick_ggx_geometry_function(float roughness, vec3 normal,
        vec3 frag_to_camera, vec3 frag_to_light)
{
    float reflection_coefficient = pow(roughness + 1.0, 2.0) / 8.0;
    return schlick_ggx_geometry_function(normal, frag_to_camera, reflection_coefficient)
        * schlick_ggx_geometry_function(normal, frag_to_light, reflection_coefficient);
}

vec3 fresnel_schlick_fresnel_equation(vec3 albedo, float metalness,
        vec3 halfway_vector, vec3 frag_to_camera)
{
    vec3 base_reflection_coefficient = mix(vec3(0.04), albedo, metalness);
    return base_reflection_coefficient + (1.0 - base_reflection_coefficient)
        * pow(1.0 - max(dot(halfway_vector, frag_to_camera), 0.0), 5.0);
}

vec3 cook_torrance_brdf(vec3 albedo, float roughness, float metalness, vec3 normal,
        vec3 frag_position, vec3 camera_position, vec3 light_position)
{
    vec3 frag_to_camera = normalize(camera_position - frag_position);
    vec3 frag_to_light = normalize(light_position - frag_position);
    vec3 halfway_vector = normalize(frag_to_camera + frag_to_light);

    float normal_distribution = trowbridge_reitz_ggx_normal_distribution_function(roughness,
            normal, halfway_vector);
    float microfacet_shadowing =  smith_schlick_ggx_geometry_function(roughness, normal,
            frag_to_camera, frag_to_light);
    vec3 reflection_ratio = fresnel_schlick_fresnel_equation(albedo, metalness,
            halfway_vector, frag_to_camera);
    vec3 refraction_ratio = (vec3(1.0) - reflection_ratio) * (1.0 - metalness);

    vec3 refraction = refraction_ratio * lambertian_diffuse(albedo);
    vec3 reflection = (normal_distribution * microfacet_shadowing * reflection_ratio)
        / (4.0 * max(dot(frag_to_camera, normal), 0.0)
                * max(dot(frag_to_light, normal), 0.0) + 0.0001);
    return refraction + reflection;
}
