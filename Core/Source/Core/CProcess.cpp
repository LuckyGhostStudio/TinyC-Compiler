#include <cstdio>
#include <atomic>

#include "Compiler.h"

namespace Compiler
{
	CompileProcess* CreateCompileProcess(const char* fileName, const char* outFileName, int flags)
	{
		FILE* file = fopen(fileName, "r");		// 输入文件
		if (!file) {
			return nullptr;
		}

		FILE* outFile = nullptr;
		if (outFileName) {
			outFile = fopen(outFileName, "w");	// 输出文件
			if (!outFileName) {
				return nullptr;
			}
		}

		/* 初始化编译过程 */
		CompileProcess* process = (CompileProcess*)calloc(1, sizeof(CompileProcess));	// 编译过程
		if (!process) {
			return nullptr;
		}
		process->flags = flags;
		process->cFile.file = file;
		process->outFile = outFile;

		return process;
	}
}