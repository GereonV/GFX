#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoord;
uniform float uZ;
out vec2 texCoord;

void main() {
	gl_Position = vec4(inPos, uZ, 1);
	texCoord = inTexCoord;
}

