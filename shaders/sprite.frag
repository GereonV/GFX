#version 420 core
in vec2 texCoord;
layout (binding = 0) uniform sampler2D uTex;
out vec4 outColor;

void main() {
	outColor = texture(uTex, texCoord);
}

