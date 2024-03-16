#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 PROJECTION_MATRIX;
    mat4 VIEW_MATRIX;
};

layout (std430, binding = 1) buffer  Instance
{
    mat4 INSTANCE_TRANSFORMS[];
};

void main()
{
    mat4 MODEL_MATRIX = INSTANCE_TRANSFORMS[gl_InstanceID];

    vs_out.Normal    = aNormal;
    vs_out.TexCoords = aTexCoords;

    gl_Position = PROJECTION_MATRIX * VIEW_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}