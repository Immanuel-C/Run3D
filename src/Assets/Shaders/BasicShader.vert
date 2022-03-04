#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 normal;
out vec3 nonModifiedNormal;
out vec2 texCoords;
out vec3 cubeTexCoords;
out vec3 fragPos;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

void main() {
	// gl_Position is the output for the vertex position
	gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
	// lighting stuff
	fragPos = vec3(uTransform * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(uTransform))) * aNormal;  
	nonModifiedNormal = aNormal;
	texCoords = aTexCoords;
	// for cubeMaps
	cubeTexCoords = aPos;
}