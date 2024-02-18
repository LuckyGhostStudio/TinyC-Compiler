#pragma once

#include "CompileProcess.h"
#include "Lexer.h"

namespace Compiler
{
	/// <summary>
	/// ����״̬
	/// </summary>
	enum class CompileState
	{
		Successfully,		// ����ɹ�
		FailedWithErrors,	// �������
	};

	/// <summary>
	/// �����ļ�
	/// </summary>
	/// <param name="fileName">�����ļ���</param>
	/// <param name="outFileName">����ļ���</param>
	/// <param name="flags">�ļ����뷽ʽ�ı�־</param>
	/// <returns>������״̬</returns>
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags);
}