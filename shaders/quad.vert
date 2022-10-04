#version 430 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inCoord;
layout (location = 0) uniform mat4 uTransformation;
out vec2 coord;

void main() {
	gl_Position = uTransformation * vec4(inPos, 0, 1);
	coord = inCoord;
}

