#version 460

out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D screenSampler;

void main()
{
    vec3 color = texture(screenSampler, fs_in.TexCoords).rgb;
    FragColor  = vec4(color, 1.0);
}