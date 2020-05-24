#version 330 core

out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube cubemap;

void main()
{
	vec3 envcolor = texture(cubemap, TexCoord).rgb;

	envcolor = envcolor / (envcolor + vec3(1.0));
	envcolor = pow(envcolor, vec3(1.0/2.2));

	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	FragColor = vec4(envcolor, 1.0);
}