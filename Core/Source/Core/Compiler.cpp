#include "Compiler.h"

namespace Compiler
{
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags)
	{
		CompileProcess* process = CreateCompileProcess(fileName, outFileName, flags);	// �����������
		if (!process) {
			return CompileState::FailedWithErrors;
		}

		// TODO: �ʷ�����

		// TODO: �﷨����

		// TODO: �������

		// TODO: �м��������

		// TODO: Ŀ���������...

		return CompileState::FileCompiledOK;	// �������
	}
}