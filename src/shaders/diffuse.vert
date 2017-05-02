#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 n;
layout (location = 2) in vec2 tex;

out vec3 normal;
out vec3 frag_position;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(pos, 1.0f);
    normal = n;
    frag_position = pos;
}