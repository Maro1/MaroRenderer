#version 330 core

#define MAX_POINT_LIGHTS 10

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;
in mat3 TBN;

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
uniform DirLight directionLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

// textureSamples
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform bool usingNormal;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm;
    if(usingNormal)
    {
        norm = texture(normalMap, TexCoord).rgb;
        norm.b = 1 - norm.b;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(TBN * norm);
    }
    else
    {
        norm = Normal;
	}

	vec3 viewDir = normalize((viewPos - FragPos)*TBN);

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
	vec3 lightDirection = normalize(-light.direction*TBN);

	// Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);

	// Specular
	vec3 reflectDir = reflect(-lightDirection, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	// Results 
	vec3 ambient = light.ambient * vec3(texture(diffuseMap, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, TexCoord));
    vec3 specular = light.specular * spec;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDirection = normalize(TBN*light.position - fragPos*TBN);

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);

    // Specular
    vec3 halfwayDir = normalize(lightDirection + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
	
    // Results
    vec3 ambient = light.ambient * vec3(texture(diffuseMap, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(diffuseMap, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(diffuseMap, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

