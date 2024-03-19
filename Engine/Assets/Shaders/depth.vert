#version 460
layout (location = 0) in vec3 aPos;

layout (std430, binding = 0) buffer  Instance
{
    mat4 INSTANCE_TRANSFORMS[];
};

layout (std140, binding = 2) uniform Shadow
{
	mat4 LIGHT_TRANSFORM_MATRIX;
};

void main()
{
	mat4 MODEL_MATRIX = INSTANCE_TRANSFORMS[gl_InstanceID];

	gl_Position = LIGHT_TRANSFORM_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}