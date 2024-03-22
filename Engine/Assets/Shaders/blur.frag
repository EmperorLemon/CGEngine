#version 460

out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
} fs_in;

const float WEIGHTS[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
const float SAMPLE_DISTANCE = 0.2;

uniform bool horizontal;
uniform sampler2D imageSampler;

void main()
{
    // Get size of single texel
    vec2 offset = 1.0 / textureSize(imageSampler, 0);
    vec3 result = texture(imageSampler, fs_in.TexCoords).rgb * WEIGHTS[0];

    if (horizontal)
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(imageSampler, fs_in.TexCoords + vec2(offset.x * i * SAMPLE_DISTANCE, 0.0)).rgb * WEIGHTS[i];
            result += texture(imageSampler, fs_in.TexCoords - vec2(offset.x * i * SAMPLE_DISTANCE, 0.0)).rgb * WEIGHTS[i];
        }
    }
    else
    {
        for (int i = 1; i < 5; ++i)
        {
            result += texture(imageSampler, fs_in.TexCoords + vec2(0.0, offset.y * i * SAMPLE_DISTANCE)).rgb * WEIGHTS[i];
            result += texture(imageSampler, fs_in.TexCoords - vec2(0.0, offset.y * i * SAMPLE_DISTANCE)).rgb * WEIGHTS[i];
        }
    }

    FragColor = vec4(result, 1.0);
}