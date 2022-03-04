#version 450 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;
in vec3 nonModifiedNormal;

uniform vec3 lightPos;
uniform vec3 lightColour;
uniform float lightStrength;
uniform vec3 camPos;

out vec4 outColor;

void main() {
	// ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColour;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour * lightStrength;

	// specular
    float specularStrength = 0.5;
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColour * lightStrength; 

	outColor = vec4(0.1, 0.1, 0.1, 1.0) * vec4((ambient + diffuse + specular), 1.0);
}