#version 330 core

in vec3 normal;
in vec3 frag_pos;

out vec4 color;

uniform vec3 view_pos;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform samplerCube skybox;

void main() {
    float ratio = 1 / 1.33;
    vec3 I = normalize(frag_pos - view_pos);
    vec3 R = refract(I, normalize(normal), ratio);
    vec3 object_color = vec3(texture(skybox, R));

    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(light_pos - frag_pos);

    float specular_strength = 0.5f;
    vec3 viewDir = normalize(view_pos - frag_pos);
    vec3 reflectDir = reflect(-light_dir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specular_strength * spec * light_color;

    vec3 result = specular + object_color;
    color = vec4(result, 1.0f);
}
