#version 330 core

const float PI = 3.1415926535897932384626433832795;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 light_direction;

uniform vec3 light_direction2;

uniform sampler2D textureSampler;

const float shading_ambient_strength    = 0.2;
const float shading_specular_strength   = 0.5;
const float shading_diffuse_strength    = 0.3;

uniform float light_cutoff_outer;
uniform float light_cutoff_inner;
uniform float light_near_plane;
uniform float light_far_plane;

uniform vec3 view_position;

uniform vec3 view_position2;

in vec3 fragment_position;
in vec3 fragment_normal;
in vec2 TexCoords;

out vec4 result;

vec3 ambient_color(vec3 light_color_arg) {
    return shading_ambient_strength * texture(textureSampler, TexCoords).rgb;
}

vec3 diffuse_color(vec3 light_color_arg, vec3 light_position_arg, vec3 light_direction_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    return vec3(shading_diffuse_strength) * max(dot(normalize(fragment_normal), light_direction_arg), 0.0);
}

vec3 specular_color(vec3 light_color_arg, vec3 light_position_arg, vec3 light_direction_arg, vec3 view_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    vec3 view_direction = normalize(view_position_arg - fragment_position);
    vec3 reflect_light_direction = reflect(light_direction_arg, normalize(fragment_normal));

    return vec3(shading_specular_strength) * pow(max(dot(reflect_light_direction, view_direction), 0.0),32);
}

float spotlight_scalar() {
    float theta = dot(light_direction, normalize(fragment_position - light_position));
    float epsilon = light_cutoff_inner - light_cutoff_outer;
    return clamp((theta - light_cutoff_outer) / epsilon, 0.0, 1.0);
}

void main()
{
    vec3 ambient = ambient_color(light_color);
    vec3 diffuse = diffuse_color(light_color, light_position, light_direction);
    vec3 specular = specular_color(light_color, light_position, light_direction, view_position);
    vec3 diffuse2 = diffuse_color(light_color, light_position, light_direction2);
    vec3 specular2 = specular_color(light_color, light_position, light_direction2, view_position2);
    diffuse  *= spotlight_scalar();
    specular *= spotlight_scalar();
    diffuse2  *= spotlight_scalar();
    specular2 *= spotlight_scalar();
    
    vec3 color = ((specular2 + diffuse2) + (specular + diffuse + ambient)) * texture(textureSampler, TexCoords).rgb;

    result = vec4(color, 1.0);
}

