#version 460

out vec4 FragColor;

in VS_OUT {
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform vec3 uMeshColor = vec3(1.0);

void main()
{
	FragColor = vec4(uMeshColor, 1.0);
}