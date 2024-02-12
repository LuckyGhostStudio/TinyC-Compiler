#include <cstdio>
#include <atomic>

#include "Compiler.h"

namespace Compiler
{
	CompileProcess* CreateCompileProcess(const char* fileName, const char* outFileName, int flags)
	{
		FILE* file = fopen(fileName, "r");		// �����ļ�
		if (!file) {
			return nullptr;
		}

		FILE* outFile = nullptr;
		if (outFileName) {
			outFile = fopen(outFileName, "w");	// ����ļ�
			if (!outFileName) {
				return nullptr;
			}
		}

		/* ��ʼ��������� */
		CompileProcess* process = (CompileProcess*)calloc(1, sizeof(CompileProcess));	// �������
		if (!process) {
			return nullptr;
		}
		process->flags = flags;
		process->cFile.file = file;
		process->outFile = outFile;

		return process;
	}
}