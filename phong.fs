#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

in vec3 fragmentPosition;
in vec3 normalVector;
in vec3 vertexColor;
in vec2 textureCoordinates;

out vec4 color;

uniform Material material;
uniform Light light;

uniform vec3 viewPosition;

void main () {
	// Vectors
	vec3 normalDirection = normalize(normalVector);
	vec3 lightDirection = normalize(light.position - fragmentPosition); 
	vec3 viewDirection = normalize(viewPosition - fragmentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normalDirection);  
	
	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoordinates));

	// Diffuse
	float diff = max(dot(normalDirection, lightDirection), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, textureCoordinates)));

	// Specular
	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, textureCoordinates)));

	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}