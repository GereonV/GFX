#version 430 core

in vec2 coord;
uniform sampler2D uTex; // implictly texture unit 0 (binding = 0)
layout (location = 1) uniform float uAlphaThreshold;
layout (location = 2) uniform vec2 uPosition; // bottom left
layout (location = 3) uniform vec2 uRelativeSize = vec2(1, 1);
out vec4 outColor;

void main() {
	vec2 texCoord = uPosition + coord * uRelativeSize;
	outColor = texture(uTex, texCoord);
	if(outColor.a <= uAlphaThreshold)
		discard;
}
