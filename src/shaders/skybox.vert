#version 330 core

layout (location = 0) in vec3 position;

out vec3 tex_coords;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec4 pos = projection * view * vec4(position, 1.f);
	gl_Position = pos.xyww;
	tex_coords = position;
}
