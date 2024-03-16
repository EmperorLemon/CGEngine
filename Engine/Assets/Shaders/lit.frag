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
    vec4  direction;

    uint  type;

	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

layout (std140, binding = 2) uniform CameraBuffer
{
    vec3 VIEW_POSITION;
};

layout (std430, binding = 3) buffer  LightBuffer
{
    uint  NUM_LIGHTS;
    Light LIGHTS[8];
};

uniform Material material;

vec3 Lighting(in vec3 normal, in vec3 albedo)
{
    vec3 ambient  = vec3(0.0);
    vec3 diffuse  = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        Light light           = LIGHTS[i];
        vec3  lightPos        = vec3(2.0, 1.0, 10.0);

        vec3  lightDir        = normalize(lightPos.xyz - fs_in.FragPos);
        vec3  viewDir         = normalize(VIEW_POSITION - fs_in.FragPos);
        vec3  reflectDir      = reflect(-lightDir, normal);
        vec3  halfwayDir      = normalize(lightDir + viewDir);

        float ambientFactor   = 0.1;
        ambient               = ambientFactor * albedo;                   

        float diffuseFactor   = max(dot(normal, lightDir), 0.0); 
        diffuse               = diffuseFactor * albedo;                  

        float specularFactor  = pow(max(dot(normal, halfwayDir), 0.0), 32);
        specular              = specularFactor * vec3(0.2);

        if (light.type == SPOT_LIGHT)
        {
            float theta       = dot(lightDir, normalize(-light.direction.xyz));
            float epsilon     = light.cutOff - light.outerCutOff;
            float intensity   = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

            diffuse  *= intensity;
            specular *= intensity;
        }

        if (light.type == POINT_LIGHT || light.type == SPOT_LIGHT)
        {
            float distance        = length(lightPos.xyz - fs_in.FragPos);
            float attenuation     = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));

            ambient  *= attenuation;
            diffuse  *= attenuation;
            specular *= attenuation;
        }
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

    vec3 lighting = Lighting(normal, albedo.rgb);

	FragColor = vec4(lighting, 1.0);
}