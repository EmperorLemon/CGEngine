#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 oVertexColor;

void main()
{
	oVertexColor = aColor;

	gl_Position = vec4(aPos, 1.0);
}