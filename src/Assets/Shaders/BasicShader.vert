#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoords;

out vec3 fragColor;
out vec2 texCoords;
out vec3 cubeTexCoords;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

void main() {
	gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
	fragColor = aColor;
	texCoords = aTexCoords;
	cubeTexCoords = aPos;
}