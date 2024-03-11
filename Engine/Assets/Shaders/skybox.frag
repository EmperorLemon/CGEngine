#version 460

out vec4 FragColor;

in VS_OUT {
    vec3 TexCoords;
} fs_in;

uniform samplerCube skyboxSampler;

void main()
{
	FragColor = texture(skyboxSampler, fs_in.TexCoords);
}