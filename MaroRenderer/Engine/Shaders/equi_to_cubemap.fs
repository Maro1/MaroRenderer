#version 330 core

out vec4 FragColor;

in vec3 WorldPos;

uniform sampler2D Skybox;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}


void main()
{
	vec2 uv = SampleSphericalMap(normalize(WorldPos)); // make sure to normalize localPos
    vec3 color = texture(Skybox, uv).rgb;

	FragColor = vec4(color, 1.0);
}