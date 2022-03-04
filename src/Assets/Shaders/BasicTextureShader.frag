#version 450 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D textureSampler;

uniform vec3 lightPos;
uniform vec3 lightColour;
uniform float lightStrength;
uniform vec3 camPos;

out vec4 outColor;

void main() {
	// ambient
    float ambientStrength = 0.75;
    vec3 ambient = vec3(ambientStrength);
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;

	// specular
    float specularStrength = 0.5;
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * lightColour;  

    outColor = texture(textureSampler, texCoords) * ( vec4( (ambient + diffuse + specular) , 1.0) * lightStrength );
}