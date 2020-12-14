#version 450 core

#define MAX_POINT_LIGHTS 10

out vec4 outColor;


layout (location=0) in Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	mat3 tangentBasis;
} vertex;

uniform int color;

void main()
{
	outColor = vec4(color/255.0, color/255.0, color/255.0, 1.0);
}



