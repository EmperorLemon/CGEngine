#pragma once

class Shader
{
	virtual ~Shader() = default;

	virtual void Bind()   const = 0;
	virtual void Unbind() const = 0;
};