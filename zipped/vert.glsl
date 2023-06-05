#version 330 core

in vec2 position;
in vec3 color;
in vec2 uv;

out vec3 pass_color;
out vec2 pass_uv;

void main() {
	pass_uv = uv;
	pass_color = color;
	gl_Position = vec4(position, 0.0, 1.0);
}
