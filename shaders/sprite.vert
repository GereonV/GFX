#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoord;
uniform mat4 uModel;
out vec2 texCoord;

void main() {
	gl_Position = uModel * vec4(inPos, 0, 1);
	texCoord = inTexCoord;
}

