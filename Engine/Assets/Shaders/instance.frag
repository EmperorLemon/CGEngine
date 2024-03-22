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
    vec3 ViewPos;
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
    const int halfKernelWidth = 3;

    // PCF: Percentage-Closer Filtering
    for (int x = -halfKernelWidth; x <= halfKernelWidth; ++x)
    {
        for (int y = -halfKernelWidth; y <= halfKernelWidth; ++y)
        {
            float pcfDepth = texture(shadowMapSampler, projCoords.xy + vec2(x, y) * texelSize).r;
            
            // Check whether current frag pos is in shadow
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= ((halfKernelWidth * 2 + 1) * (halfKernelWidth * 2 + 1));

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
    diffuse               = light.diffuseColor.rgb * diffuseFactor * albedo;                  

    float specularFactor  = pow(max(dot(normal, halfwayDir), 0.0), 32);
    specular              = light.specularColor.rgb * specularFactor;

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

    //float shadow = CalculateShadow(normal, lightDir);

    //return (ambient + (1.0 - shadow) * (diffuse + specular)) * albedo;
    return (ambient + diffuse + specular) * albedo;
}

void main()
{
    vec4 baseColorTexture     = texture(baseAlbedoSampler, fs_in.TexCoords * material.uv_scale + material.uv_offset);
    vec4 baseNormalTexture    = texture(baseNormalSampler, fs_in.TexCoords * material.uv_scale + material.uv_offset);
    vec4 baseOcclusionTexture = texture(baseOcclusionSampler, fs_in.TexCoords * material.uv_scale + material.uv_offset);

    vec3 albedo = baseColorTexture.rgb;
    // Transform normal vector to range [-1, 1]
    //vec3 normal = normalize(baseNormalTexture.rgb * 2.0 - 1.0);
    vec3 normal   = normalize(fs_in.Normal);

    float metallic  = material.metallicFactor;
    float roughness = material.roughnessFactor;

    vec3 ambient = 0.0 * albedo;
    vec3 lighting = vec3(0.0);
    vec3 viewDir = normalize(fs_in.ViewPos - fs_in.FragPos);

    for (int i = 0; i < 4; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(LIGHTS[i].direction.xyz - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 result = LIGHTS[i].diffuseColor.rgb * diff * albedo;      
        
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(fs_in.FragPos - LIGHTS[i].direction.xyz);
        result *= 1.0 / (distance * distance);
        lighting += result;
    }

    vec3 result = ambient + lighting;
    //result = pow(result, vec3(1.0 / 2.2));

    // Check if fragment output is higher than threshold (sRGB)
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));

    if (brightness > 1.0)
        HDRGlowColor = vec4(result, 1.0);
    else
        HDRGlowColor = vec4(vec3(0.0), 1.0);

	HDRFragColor = vec4(result, 1.0);
}