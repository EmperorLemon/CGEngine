#version 460
layout(location = 0) in vec3 aPos;

out VS_OUT {
    vec3 TexCoords;
} vs_out;

layout (std140, binding = 1) uniform Camera
{
    mat4 PROJECTION_MATRIX;
};

uniform mat4 SKYBOX_VIEW_MATRIX;

void main()
{
    vs_out.TexCoords = aPos;
    vec4 pos = PROJECTION_MATRIX * SKYBOX_VIEW_MATRIX * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}