#version 460

out vec4 FragColor;

const float GAMMA = 2.2;
const float EXPOSURE = 1.0;

in VS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D HDRColorSampler;
uniform sampler2D HDRBloomSampler;

void main()
{
    vec3 HDRColor = texture(HDRColorSampler, fs_in.TexCoords).rgb;
    vec3 HDRBloomColor = texture(HDRBloomSampler, fs_in.TexCoords).rgb;

    // additive blending
    HDRColor += HDRBloomColor;

    // Reinhard tone mapping
    vec3 result = vec3(1.0) - exp(-HDRColor * EXPOSURE);

    // Gamma correction
    result = pow(result, vec3(1.0 / GAMMA));

    FragColor = vec4(result, 1.0);
}