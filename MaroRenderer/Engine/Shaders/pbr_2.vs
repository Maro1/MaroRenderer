#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;

layout (location = 0) out Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    mat3 tangentBasis; 
} vertex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat3 modelMat = mat3(transpose(inverse(model))); 
    vertex.position = vec3(model * vec4(position, 1.0));
    vertex.normal = normalize(modelMat * normal);
    vertex.texCoord = texCoord;
    vertex.tangentBasis = mat3(model) * mat3(tangent, biTangent, normal);

    
    
    gl_Position = projection * view * vec4(vertex.position, 1.0);
}