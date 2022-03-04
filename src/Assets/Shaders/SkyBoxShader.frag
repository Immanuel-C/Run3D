#version 450 core

in vec3 cubeTexCoords;

uniform samplerCube textureSampler;

out vec4 outColor;

void main() {
	outColor = texture(textureSampler, cubeTexCoords);
}