#version 330 core

#define MAX_POINT_LIGHTS 10

out vec4 FragColor;

uniform samplerCube irradianceMap;

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
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform bool usingNormal;
uniform bool usingDiffuse;

const float PI = 3.14159265359;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 FresnelSchlick(float cosAngle, vec3 reflectionCoeff)
{
	return reflectionCoeff + (1.0 -reflectionCoeff ) * pow(1.0 - cosAngle, 5.0);
}

vec3 FresnelSchlickRoughness(float cosAngle, vec3 reflectionCoeff, float roughness)
{
	return reflectionCoeff + (max(vec3(1.0 - roughness), reflectionCoeff) - reflectionCoeff) * pow(1.0 - cosAngle, 5.0);
}

float NormalDist(float NdotH, float coeff)
{
	float a = coeff * coeff;
	float a2 = a * a;
	float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;
	return a2 / max(denom, 0.0000001);
}

float GeometrySmith(float NdotV, float NdotL, float coeff)
{
	float r = coeff + 1.0;
	float k = (r * r) / 8.0;
	float ggx1 = NdotV / (NdotV * (1.0 - k) + k);
	float ggx2 = NdotL / (NdotL * (1.0 - k) + k);
	return ggx1 * ggx2;
}

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

	vec3 L0 = vec3(0.0);

	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		if(pointLights[i].diffuse != vec3(0)) 
		{
			L0 += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
		}
	}

	vec3 irradiance = texture(irradianceMap, norm).rgb;
	vec3 diffuse = irradiance * texture(diffuseMap, TexCoord).rgb;

	vec3 ambient = vec3(0.03) * diffuse;

	vec3 color = ambient + L0;

	color = color / (color + vec3(1.0/2.2));
	color = pow(color, vec3(1.0/2.2));

	FragColor = vec4(color, 1.0);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 albedo = texture(diffuseMap, TexCoord).rgb;
	float metallic = texture(metallicMap, TexCoord).r;
	float roughness = texture(roughnessMap, TexCoord).r;

	vec3 baseRef = mix(vec3(0.04), albedo, metallic);

    vec3 L = normalize(TBN*light.position - fragPos*TBN);
	vec3 H = normalize(viewDir + L);
	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist)); 
	vec3 radiance = light.diffuse * attenuation;

    float NdotV = max(dot(normal, viewDir), 0.0000001);
	float NdotL = max(dot(normal, L), 0.0000001);
	float HdotV = max(dot(H, viewDir), 0.0);
	float NdotH = max(dot(normal, H), 0.0);

	float D = NormalDist(NdotH, roughness);
	float G = GeometrySmith(NdotV, NdotL, roughness);
	vec3 F = FresnelSchlick(HdotV, baseRef);

	vec3 spec = D * G * F;
	spec /= 4.0 * NdotV * NdotL;

	vec3 kD = vec3(1.0) - F;

	kD *= 1.0 - metallic;

	return (kD * albedo / PI + spec) * radiance * NdotL;
}

