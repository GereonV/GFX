#version 430 core
in vec2 texCoord;
uniform sampler2D uTex; // implictly texture unit 0 (binding = 0)
layout (location = 1) uniform float uAlphaThreshold;
out vec4 outColor;

void main() {
	outColor = texture(uTex, texCoord);
	if(outColor.a <= uAlphaThreshold)
		discard;
}

