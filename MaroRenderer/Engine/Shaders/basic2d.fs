#version 330 core

#define MAX_POINT_LIGHTS 10

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

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
  
uniform vec3 viewPos; 
uniform sampler2D sampleTexture;
uniform DirLight directionLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcDirLight(directionLight, norm, viewDir);

	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if(pointLights[i].diffuse != vec3(0)) 
		{
			result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
		}
	}

	FragColor = vec4(result, 1.0);
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) 
{
	vec3 lightDirection = normalize(-light.direction);

	// Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);

	// Specular
    vec3 reflectDir = reflect(-lightDirection, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	// Results 
	vec3 ambient = light.ambient * vec3(texture(sampleTexture, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(sampleTexture, TexCoord));
    vec3 specular = light.specular * spec;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDirection = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
	
    // Results
    vec3 ambient = light.ambient * vec3(texture(sampleTexture, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(sampleTexture, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(sampleTexture, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

