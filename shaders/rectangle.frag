#version 430 core

layout (location = 1) uniform vec4 uColor;
out vec4 outColor;

void main() {
	outColor = uColor;
}
