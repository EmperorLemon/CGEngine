#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

void main()
{
    vs_out.FragPos   = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal    = aNormal;
    vs_out.TexCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}