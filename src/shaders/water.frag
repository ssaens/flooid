#version 330 core

in vec3 normal;
in vec3 frag_pos;

out vec4 color;

uniform vec3 view_pos;
uniform samplerCube skybox;

void main() {
    float ratio = 1 / 1.33;
    vec3 I = normalize(frag_pos - view_pos);
    vec3 R = refract(I, normalize(normal), ratio);
    vec3 object_color = vec3(texture(skybox, R));

    color = vec4(object_color, 1.0f);
}
