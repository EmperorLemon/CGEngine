#version 460

out vec4 FragColor;

in vec3 oVertexColor;

void main()
{
	FragColor = vec4(oVertexColor, 1.0);
}