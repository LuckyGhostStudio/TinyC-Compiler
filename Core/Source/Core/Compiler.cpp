#include "Compiler.h"

namespace Compiler
{
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags)
	{
		CompileProcess* process = CreateCompileProcess(fileName, outFileName, flags);	// 创建编译过程
		if (!process) {
			return CompileState::FailedWithErrors;
		}

		// TODO: 词法分析

		// TODO: 语法分析

		// TODO: 语义解析

		// TODO: 中间代码生成

		// TODO: 目标代码生成...

		return CompileState::FileCompiledOK;	// 编译完成
	}
}