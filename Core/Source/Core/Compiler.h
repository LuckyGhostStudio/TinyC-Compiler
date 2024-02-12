#pragma once

#include <cstdio>

namespace Compiler
{
	enum class CompileState
	{
		FileCompiledOK,		// �ļ�����ɹ�
		FailedWithErrors,	// ����
	};

	/// <summary>
	/// �������
	/// </summary>
	struct CompileProcess
	{
		int flags;	// �ļ����뷽ʽ�ı�־

		/// <summary>
		/// �����ļ��������
		/// </summary>
		struct InputFileCompileProcess
		{
			FILE* file;					// �ļ�
			const char* absolutePath;	// ����·��
		} cFile;

		FILE* outFile;	// ����ļ�
	};

	/// <summary>
	/// �����ļ�
	/// </summary>
	/// <param name="fileName">�����ļ���</param>
	/// <param name="outFileName">����ļ���</param>
	/// <param name="flags">�ļ����뷽ʽ�ı�־</param>
	/// <returns>������״̬</returns>
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags);

	/// <summary>
	/// �����������
	/// </summary>
	/// <param name="fileName">�����ļ���</param>
	/// <param name="outFileName">����ļ���</param>
	/// <param name="flags">�ļ����뷽ʽ�ı�־</param>
	/// <returns>�������</returns>
	CompileProcess* CreateCompileProcess(const char* fileName, const char* outFileName, int flags);
}