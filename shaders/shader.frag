#version 420 core
out vec4 outColor;

in vec2 texCoord;

layout (binding = 0) uniform sampler2D uTexture1;
layout (binding = 1) uniform sampler2D uTexture2;

void main() {
	outColor = mix(texture(uTexture1, texCoord), texture(uTexture2, texCoord), .2);
}

