#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 5) in mat4 aInstanceMatrix; 

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
};

uniform bool USE_INSTANCING = true;

uniform mat4  MODEL_MATRIX;
uniform mat3 NORMAL_MATRIX;

void main()
{
    vs_out.FragPos   = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    vs_out.Normal    = NORMAL_MATRIX * aNormal;
    vs_out.TexCoords = aTexCoords;

    if (USE_INSTANCING)
        gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * aInstanceMatrix * vec4(aPos, 1.0);
    else
        gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}