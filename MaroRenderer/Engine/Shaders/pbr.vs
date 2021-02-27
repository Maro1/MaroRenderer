#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBiTangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    mat3 modelMat = mat3(transpose(inverse(model))); 

    vec3 T = normalize(modelMat * aTangent);
    vec3 B = normalize(modelMat * aBiTangent);
    Normal = normalize(modelMat * aNormal);
    TBN = mat3(T, B, Normal);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord = aTexCoord;
}
