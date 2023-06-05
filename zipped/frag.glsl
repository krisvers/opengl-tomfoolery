#version 330 core

in vec3 pass_color;
in vec2 pass_uv;

out vec4 out_color;

uniform sampler2D tex;

void main() {
	out_color = texture(tex, pass_uv);
}
