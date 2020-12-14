#version 430 core

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

// IBL Maps
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfMap;

// Textures
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform bool usingNormal;
uniform bool usingDiffuse;

uniform vec3 viewPos; 
uniform DirLight directionLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

const float PI = 3.14159265359;
const vec3 FDialectric = vec3(0.04); 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoord);
    vec2 st2 = dFdy(TexCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 FresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
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

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(float NdotV, float NdotL, float coeff)
{
	float r = coeff + 1.0;
	float k = (r * r) / 8.0;

	float ggx2 = GeometrySchlickGGX(NdotV, coeff);
    float ggx1 = GeometrySchlickGGX(NdotL, coeff);

    return ggx1 * ggx2;
}

void main()
{

	vec3 albedo = pow(texture(albedoMap, TexCoord).rgb, vec3(2.2));
	float metallic = texture(metallicMap, TexCoord).r;
	float roughness = texture(roughnessMap, TexCoord).r;

	vec3 norm;
    if(usingNormal)
    {
        norm = getNormalFromMap();
    }
    else
    {
        norm = Normal;
	}

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(viewDir, norm);

	vec3 L0 = viewDir;

    float cosL0 = max(0.0, dot(norm, L0));

    vec3 Lr = 2.0 * cosL0 * norm - L0;


	vec3 irradiance = texture(irradianceMap, norm).rgb;

    vec3 F0 = mix(FDialectric, albedo, metallic);

    vec3 F = FresnelSchlick(F0, cosL0);
    
    vec3 kD = mix(vec3(1.0) - F, vec3(0.0), metallic);
    
    vec3 diffuse = kD * albedo * irradiance;

    int specularTextureLevels = textureQueryLevels(prefilterMap);
    vec3 specularIrradiance = textureLod(prefilterMap, Lr, roughness * specularTextureLevels).rgb;

    vec2 specularBRDF = texture(brdfMap, vec2(cosL0, roughness)).rg;

    vec3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

    vec3 color = diffuse + specularIBL;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1/3.3)); 

    FragColor = vec4(color , 1.0);
} 



