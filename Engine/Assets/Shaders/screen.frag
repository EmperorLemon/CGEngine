#version 460

out vec4 FragColor;

const float GAMMA = 2.2;
const float EXPOSURE = 1.0;

in VS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D HDRColorSampler;

void main()
{
    vec3 HDRColor = texture(HDRColorSampler, fs_in.TexCoords).rgb;

    // Reinhard tone mapping
    vec3 mapped = vec3(1.0) - exp(-HDRColor * EXPOSURE);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0 / GAMMA));

    FragColor  = vec4(mapped, 1.0);
}