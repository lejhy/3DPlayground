#version 330 core
#define NR_POINT_LIGHTS 4  
#define NR_DIR_LIGHTS 1 
#define NR_SPOT_LIGHTS 1

struct Material {
	sampler2D texture_diffuse0;
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_specular0;
	sampler2D texture_specular1;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform vec3 viewPos;

out vec4 color;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight (SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main () {
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result;
	for (int i = 0; i < NR_DIR_LIGHTS; i++) {
		result += calcDirLight(dirLights[i], normal, viewDir);
	}
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		result += calcPointLight(pointLights[i], normal, FragPos, viewDir);
	}
	for (int i = 0; i < NR_SPOT_LIGHTS; i++) {
		result += calcSpotLight(spotLights[i], normal, FragPos, viewDir);
	}
	
	color = vec4(result, 1.0f);
}

vec3 calcDirLight (DirLight light, vec3 normal, vec3 viewDir) {
	// Light direction
	vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular0, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 calcPointLight (PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// Light direction
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular0, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 calcSpotLight (SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// Light direction
	vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	// Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
	// Combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular0, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}