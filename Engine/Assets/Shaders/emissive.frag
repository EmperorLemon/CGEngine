#version 460

layout (location = 0) out vec4 HDRFragColor;
layout (location = 1) out vec4 HDRGlowColor;

uniform vec4 LIGHT_COLOR;

void main()
{
    HDRFragColor = LIGHT_COLOR * 255.0;

    float brightness = dot(HDRFragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    
    if (brightness > 1.0)
        HDRGlowColor = vec4(HDRFragColor.rgb, 1.0);
	else
		HDRGlowColor = vec4(vec3(0.0), 1.0);
}