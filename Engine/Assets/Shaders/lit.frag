#version 460

out vec4 FragColor;

in VS_OUT {
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
} fs_in;

struct Material
{
    vec4 baseColor;
    sampler2D baseColorSampler;
    sampler2D metallicRoughnessSampler;
    float metallicFactor;
    float roughnessFactor;
};

uniform Material material;