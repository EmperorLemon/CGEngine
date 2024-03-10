#version 460

out vec4 FragColor;

const uint DIRECTIONAL_LIGHT = 0;
const uint POINT_LIGHT       = 1;
const uint SPOT_LIGHT        = 2;

const float LIGHT_CONSTANT = 1.0;

in VS_OUT {
    vec3 FragPos;
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

struct Light
{
    vec4  position;
//  vec3  direction;
//  uint  type;
//	float linear;
//	float quadratic;
//	float cutOff;
//	float outerCutOff;
};

layout (std430, binding = 1) buffer LightBuffer
{
//    uint  numLights;
    Light lights[];
};

uniform Material material;

vec3 Lighting(in vec3 normal, in vec3 albedo)
{
    vec3 result = vec3(0.0);

    for (uint i = 0; i < 1; ++i)
    {
        Light light   = lights[i];

        vec3  lightDir      = normalize(light.position.xyz - fs_in.FragPos);
        
        float ambientFactor = 0.3;
        vec3  ambient       = ambientFactor * vec3(1.0); // change light color

        float diffuseFactor = max(dot(normal, lightDir), 0.0); 
        vec3  diffuse       = diffuseFactor * vec3(1.0);    // change light color

        result = (ambient + diffuse) * albedo;
    }

    return result;
}

void main()
{
    vec3 normal = normalize(fs_in.Normal);
    vec4 albedo = material.albedo;

    vec4 baseColorTexture     = texture(material.baseAlbedoSampler, fs_in.TexCoords);
    vec4 baseNormalTexture    = texture(material.baseNormalSampler, fs_in.TexCoords);
    vec4 baseOcclusionTexture = texture(material.baseOcclusionSampler, fs_in.TexCoords);

    float metallic  = material.metallicFactor;
    float roughness = material.roughnessFactor;

    //albedo = albedo * baseColorTexture;

    vec3 lighting = Lighting(normal, albedo.rgb);

	FragColor = vec4(lighting, 1.0);
}