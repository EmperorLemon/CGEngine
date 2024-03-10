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

void main()
{
    vec4 baseColor = material.baseColor;

    vec4 baseColorTexture         = texture(material.baseColorSampler, fs_in.TexCoords);
    vec4 metallicRoughnessTexture = texture(material.metallicRoughnessSampler, fs_in.TexCoords);

    float metallic  = material.metallicFactor;
    float roughness = material.roughnessFactor;

    //baseColor = mix(baseColorTexture * (1.0 - metallic), metallicRoughnessTexture, metallic);
    baseColor = baseColorTexture;

	FragColor = baseColor;
}