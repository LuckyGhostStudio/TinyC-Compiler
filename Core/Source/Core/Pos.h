#pragma once

#include <string>

namespace Compiler
{
	/// <summary>
	/// λ����Ϣ
	/// </summary>
	struct Pos
	{
		int Line = 1;			// �к�
		int Column = 1;			// �к�
		std::string FileName;	// �ļ���
	};
}