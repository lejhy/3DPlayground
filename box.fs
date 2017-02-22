#version 330 core

in vec3 fragmentPosition;
in vec3 normalVector;
in vec3 vertexColor;
in vec2 textureCoordinates;

out vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPosition;

void main () {
	vec3 normalDirection = normalize(normalVector);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition); 
	vec3 viewDirection = normalize(viewPosition - fragmentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normalDirection);  
	
	// Ambient
	float ambientStrength = 0.1f;
	vec3 ambientLight = lightColor * ambientStrength;

	// Diffuse
	float diffuseStrength = max(dot(normalDirection, lightDirection), 0.0);
	vec3 diffuseLight = lightColor * diffuseStrength;

	// Specular
	float specularStrength = 0.5f;
	int specularIntensity = 32;
	float specular = pow(max(dot(viewDirection, reflectionDirection), 0.0), specularIntensity);
	vec3 specularLight = specularStrength * specular * lightColor; 
	

	// Object color
	color = texture(texture0, textureCoordinates) * vec4(vertexColor, 1.0f);
	color = mix(color, texture(texture1, textureCoordinates), texture(texture1, textureCoordinates).a * 0.5);
	
	vec3 combinedLight = ambientLight + diffuseLight + specularLight;
	color = color * vec4(combinedLight, 1.0f);


}