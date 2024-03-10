#include "Matrix4x4.h"

namespace CGEngine::Math
{
	Mat4 Transpose(const Mat4& matrix)
	{
		return transpose(matrix);
	}

	Mat4 Inverse(const Mat4& matrix)
	{
		return inverse(matrix);
	}
}