#version 460

out vec4 FragColor;

in VS_OUT {
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
} fs_in;

struct Material
{
    vec4 albedo;
    sampler2D baseAlbedoSampler;
    sampler2D baseNormalSampler;
    sampler2D baseOcclusionSampler;
    float metallicFactor;
    float roughnessFactor;
};

uniform Material material;

void main()
{
    vec4 albedo = material.albedo;

    vec4 baseColorTexture     = texture(material.baseAlbedoSampler, fs_in.TexCoords);
    vec4 baseNormalTexture    = texture(material.baseNormalSampler, fs_in.TexCoords);
    vec4 baseOcclusionTexture = texture(material.baseOcclusionSampler, fs_in.TexCoords);

    float metallic  = material.metallicFactor;
    float roughness = material.roughnessFactor;

    albedo = albedo * baseColorTexture;

	FragColor = albedo;
}