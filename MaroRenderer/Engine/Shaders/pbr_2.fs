#version 450 core

#define MAX_POINT_LIGHTS 10

out vec4 color;

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

layout (location=0) in Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	mat3 tangentBasis;
} vertex;

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
uniform bool usingAlbedo;
uniform bool usingRoughness;
uniform bool usingMetallic;

uniform bool highlighted;

uniform vec3 viewPos; 
uniform DirLight directionLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

const float PI = 3.14159265359;
const vec3 FDialectric = vec3(0.04); 

float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	// Sample input textures to get shading model params.
	vec3 albedo;
	if (usingAlbedo)
		albedo = texture(albedoMap, vertex.texCoord).rgb;
	else
		albedo = vec3(0.9);

	float metalness;
	if (usingMetallic)
		metalness = texture(metallicMap, vertex.texCoord).r;
	else
		metalness = 0.0;

	float roughness;
	if (usingRoughness) 
		roughness = texture(roughnessMap, vertex.texCoord).r;
	else
		roughness = 1.0;

	// Outgoing light direction (vector from world-space fragment position to the "eye").
	vec3 Lo = normalize(viewPos - vertex.position);

	// Get current fragment's normal and transform to world space.
	vec3 N;
	if (usingNormal)
	{
		N = normalize(2.0 * texture(normalMap, vertex.texCoord).rgb - 1.0);
		N = normalize(vertex.tangentBasis * N);
	}
	else
	{
		N = vertex.normal;
	}
	
	// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(N, Lo));
		
	// Specular reflection vector.
	vec3 Lr = 2.0 * cosLo * N - Lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
	vec3 F0 = mix(vec3(0.04), albedo, metalness);

	// Ambient lighting (IBL).
	vec3 ambientLighting;
	{
		// Sample diffuse irradiance at normal direction.
		vec3 irradiance = texture(irradianceMap, N).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
		vec3 F = fresnelSchlick(F0, cosLo);

		// Get diffuse contribution factor (as with direct lighting).
		vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
		vec3 diffuseIBL = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level.
		int specularTextureLevels = textureQueryLevels(irradianceMap);
		vec3 specularIrradiance = textureLod(irradianceMap, Lr, roughness * specularTextureLevels).rgb;

		// Split-sum approximation factors for Cook-Torrance specular BRDF.
		vec2 specularBRDF = texture(brdfMap, vec2(cosLo, roughness)).rg;

		// Total specular IBL contribution.
		vec3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
		ambientLighting = diffuseIBL + specularIBL;
	}

	// Final fragment color.
	if (highlighted)
	{
		vec4 a = vec4(ambientLighting, 1.0);
		vec4 b = vec4(0.4, 0.7, 1.0, 1.0);
		color = mix(a, b, 0.2);
	}
	else
	{
		color = vec4(ambientLighting, 1.0);
	}
}



