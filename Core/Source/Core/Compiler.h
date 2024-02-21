#pragma once

#include "CompileProcess.h"

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
	/// ����������
	/// </summary>
	/// <param name="compiler">�������</param>
	/// <param name="message">������Ϣ</param>
	/// <param name="">��������</param>
	void CompilerError(CompileProcess* compiler, const char* message, ...);

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="compiler">�������</param>
	/// <param name="message">������Ϣ</param>
	/// <param name="">��������</param>
	void CompilerWarning(CompileProcess* compiler, const char* message, ...);

	/// <summary>
	/// �����ļ�
	/// </summary>
	/// <param name="fileName">�����ļ���</param>
	/// <param name="outFileName">����ļ���</param>
	/// <param name="flags">�ļ����뷽ʽ�ı�־</param>
	/// <returns>������״̬</returns>
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags);
}