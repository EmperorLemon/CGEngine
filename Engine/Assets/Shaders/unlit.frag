#version 460

out vec4 FragColor;

in VS_OUT {
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

void main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}