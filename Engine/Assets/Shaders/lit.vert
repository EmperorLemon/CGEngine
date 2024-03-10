#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT {
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
} vs_out;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
    vs_out.Normal    = aNormal;
    vs_out.TexCoords = aTexCoords;

	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}