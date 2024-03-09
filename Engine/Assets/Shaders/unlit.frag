#version 460

out vec4 FragColor;

in VS_OUT {
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Material
{
    vec4 baseColor;
    sampler2D baseTexture;
};

uniform Material uMaterial;

void main()
{
	FragColor =  texture(uMaterial.baseTexture, fs_in.TexCoords);
}