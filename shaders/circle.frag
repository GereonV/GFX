#version 430 core
in vec2 coord;
layout (location = 1) uniform vec4 uColor;
out vec4 outColor;

void main() {
	float x = coord.x - 0.5;
	float y = coord.y - 0.5;
	if(x * x + y * y > 0.25)
		discard;
	outColor = uColor;
}
