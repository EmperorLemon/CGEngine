#version 460

out vec4 FragColor;

const uint DIRECTIONAL_LIGHT = 0;
const uint POINT_LIGHT       = 1;
const uint SPOT_LIGHT        = 2;

const uint MAX_NUM_LIGHTS = 10;

in VS_OUT {
    vec3 ViewPos;
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
    vec4  direction;

    vec4  diffuseColor;
    vec4  specularColor;

    float constant;
    float linear;
    float quadratic;

    float innerSpotAngle;
    float outerSpotAngle;

    uint  type;
};

layout (std140, binding = 2) uniform Lights
{
    Light LIGHTS[MAX_NUM_LIGHTS];
    uint NUM_LIGHTS;
};

uniform Material material;

vec3 Lighting(in Light light, in vec3 normal, in vec3 albedo)
{
    vec3 ambient  = vec3(0.0);
    vec3 diffuse  = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 lightDir = vec3(0.0);

    if (light.type == DIRECTIONAL_LIGHT)
        lightDir          = normalize(-light.direction.xyz);
    else
        lightDir          = normalize(light.direction.xyz  - fs_in.FragPos);

    vec3  viewDir         = normalize(fs_in.ViewPos - fs_in.FragPos);
    vec3  reflectDir      =   reflect(-lightDir, normal);
    vec3  halfwayDir      = normalize(lightDir + viewDir);

    float ambientFactor   = 0.1;
    ambient               = ambientFactor * albedo;                   

    float diffuseFactor   = max(dot(normal, lightDir), 0.0); 
    diffuse               = light.diffuseColor.xyz * diffuseFactor * albedo;                  

    float specularFactor  = pow(max(dot(normal, halfwayDir), 0.0), 32);
    specular              = light.specularColor.xyz * specularFactor;

    if (light.type == POINT_LIGHT || light.type == SPOT_LIGHT)
    {
        float distance        = length(light.direction.xyz - fs_in.FragPos);
        float attenuation     = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance));// * distance));

        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
    }

    if (light.type == SPOT_LIGHT)
    {
        float theta       = dot(lightDir, normalize(-light.direction.xyz));
        float epsilon     = light.innerSpotAngle - light.outerSpotAngle;
        float intensity   = clamp((theta - light.outerSpotAngle) / epsilon, 0.0, 1.0);

        diffuse  *= intensity;
        specular *= intensity;
    }

    return (ambient + diffuse + specular) * albedo;
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

    albedo = baseColorTexture;

    vec3 result = vec3(0.0);
    
    for (int i = 0; i < NUM_LIGHTS; ++i)
        result += Lighting(LIGHTS[i], normal, albedo.rgb);

	FragColor = vec4(pow(result, vec3(1.0 / 2.2)), 1.0);
}