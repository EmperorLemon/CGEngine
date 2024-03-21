#version 460

layout (location = 0) out vec4 HDRFragColor;
layout (location = 1) out vec4 HDRGlowColor;

const uint DIRECTIONAL_LIGHT = 0;
const uint POINT_LIGHT       = 1;
const uint SPOT_LIGHT        = 2;

const uint MAX_NUM_LIGHTS = 10;

in VS_OUT {
    vec3 FragPos;
    vec4 FragLightPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Material
{
    vec4  albedo;
    float metallicFactor;
    float roughnessFactor;

    vec2 uv_scale;
    vec2 uv_offset;
};

uniform sampler2D baseAlbedoSampler;
uniform sampler2D baseNormalSampler;
uniform sampler2D baseOcclusionSampler;

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

layout (std140, binding = 3) uniform Lights
{
    Light LIGHTS[MAX_NUM_LIGHTS];
    uint NUM_LIGHTS;
};

uniform Material material;
uniform sampler2D shadowMapSampler;

float CalculateShadow(in vec3 normal, in vec3 lightDir)
{
    // Perspective divide
    vec3 projCoords = fs_in.FragLightPos.xyz / fs_in.FragLightPos.w;

    // Normalize to [0, 1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get the closest depth value from light's perspective
    float closestDepth = texture(shadowMapSampler, projCoords.xy).r;

    // Get the depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Calculate the min and max shadow bias based on a surface's normal and light direction
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(shadowMapSampler, 0);

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMapSampler, projCoords.xy + vec2(x, y) * texelSize).r;
            
            // Check whether current frag pos is in shadow
            shadow += currentDepth - bias > closestDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if (projCoords.z > 1.0) shadow = 0.0;

    return shadow;
}

vec3 Lighting(in Light light, in vec3 normal, in vec3 albedo)
{
    vec3 ambient  = vec3(0.0);
    vec3 diffuse  = vec3(0.0);
    vec3 specular = vec3(0.0);

    vec3 lightDir = vec3(0.0);
    vec3 lightPos = fs_in.TBN * light.direction.xyz;

    if (light.type == DIRECTIONAL_LIGHT)
        lightDir          = normalize(-lightPos);
    else
        lightDir          = normalize(lightPos - fs_in.TangentFragPos);

    vec3  viewDir         = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3  halfwayDir      = normalize(lightDir + viewDir);

    float ambientFactor   = 0.05;
    ambient               = ambientFactor * albedo;                   

    float diffuseFactor   = max(dot(normal, lightDir), 0.0); 
    diffuse               = light.diffuseColor.xyz * diffuseFactor * albedo;                  

    float specularFactor  = pow(max(dot(normal, halfwayDir), 0.0), 32);
    specular              = light.specularColor.xyz * specularFactor;

    if (light.type == POINT_LIGHT || light.type == SPOT_LIGHT)
    {
        float distance        = length(lightDir);
        float attenuation     = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance));// * distance));

        diffuse  *= attenuation;
        specular *= attenuation;
    }

    if (light.type == SPOT_LIGHT)
    {
        float theta       = dot(lightDir, normalize(fs_in.TBN * -light.direction.xyz));
        float epsilon     = light.innerSpotAngle - light.outerSpotAngle;
        float intensity   = clamp((theta - light.outerSpotAngle) / epsilon, 0.0, 1.0);

        diffuse  *= intensity;
        specular *= intensity;
    }

    float shadow = CalculateShadow(normal, lightDir);

    return (ambient + (1.0 - shadow) * (diffuse + specular)) * albedo;
}

void main()
{
    vec4 baseColorTexture     = texture(baseAlbedoSampler, fs_in.TexCoords * material.uv_scale + material.uv_offset);
    vec4 baseNormalTexture    = texture(baseNormalSampler, fs_in.TexCoords * material.uv_scale + material.uv_offset);
    vec4 baseOcclusionTexture = texture(baseOcclusionSampler, fs_in.TexCoords * material.uv_scale + material.uv_offset);

    vec3 albedo = baseColorTexture.rgb;
    // Transform normal vector to range [-1, 1]
    vec3 normal = normalize(baseNormalTexture.rgb * 2.0 - 1.0);
    //vec3 normal   = normalize(fs_in.Normal);

    float metallic  = material.metallicFactor;
    float roughness = material.roughnessFactor;

    vec3 result = Lighting(LIGHTS[0], normal, albedo);

    for (int i = 1; i < NUM_LIGHTS; ++i)
    {
        if (LIGHTS[i].type == POINT_LIGHT || LIGHTS[i].type == SPOT_LIGHT)
            result += Lighting(LIGHTS[i], normal, albedo);
    }

    result = pow(result, vec3(1.0 / 2.2));

	HDRFragColor = vec4(result, 1.0);

    // Check if fragment output is higher than threshold (sRGB)
    float brightness = dot(HDRFragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    if (brightness > 1.0)
        HDRGlowColor = vec4(HDRFragColor.rgb, 1.0);
    else
        HDRGlowColor = vec4(0.0);
}