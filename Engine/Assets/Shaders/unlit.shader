#DEFINE VERTEX_SHADER

#version 460 core
layout(location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0);
}

#DEFINE FRAGMENT_SHADER

#version 460 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0);
}