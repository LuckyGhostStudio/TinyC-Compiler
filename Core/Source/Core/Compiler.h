#pragma once

#include <cstdio>

namespace Compiler
{
	enum class CompileState
	{
		FileCompiledOK,		// 文件编译成功
		FailedWithErrors,	// 错误
	};

	/// <summary>
	/// 编译过程
	/// </summary>
	struct CompileProcess
	{
		int flags;	// 文件编译方式的标志

		/// <summary>
		/// 输入文件编译过程
		/// </summary>
		struct InputFileCompileProcess
		{
			FILE* file;					// 文件
			const char* absolutePath;	// 绝对路径
		} cFile;

		FILE* outFile;	// 输出文件
	};

	/// <summary>
	/// 编译文件
	/// </summary>
	/// <param name="fileName">输入文件名</param>
	/// <param name="outFileName">输出文件名</param>
	/// <param name="flags">文件编译方式的标志</param>
	/// <returns>编译器状态</returns>
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags);

	/// <summary>
	/// 创建编译过程
	/// </summary>
	/// <param name="fileName">输入文件名</param>
	/// <param name="outFileName">输出文件名</param>
	/// <param name="flags">文件编译方式的标志</param>
	/// <returns>编译过程</returns>
	CompileProcess* CreateCompileProcess(const char* fileName, const char* outFileName, int flags);
}