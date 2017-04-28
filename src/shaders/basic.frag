#version 330 core

in vec3 normal;

out vec4 color;

void main()
{
//    float ambient_strength = 0.2f;
//    vec3 light_color = vec3(1.f, 1.f, 1.f);
//    color = vec4(ambient_strength * light_color, 1.f);
    color = vec4(normal, 1.f);
}