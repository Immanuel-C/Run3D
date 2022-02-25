#version 450 core

in vec3 fragColor;
in vec2 texCoords;

uniform sampler2D textureSampler;

out vec4 outColor;

void main() {
	outColor = texture(textureSampler, texCoords);
}