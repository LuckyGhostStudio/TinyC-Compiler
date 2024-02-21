#pragma once

#include "CompileProcess.h"

namespace Compiler
{
	/// <summary>
	/// 编译状态
	/// </summary>
	enum class CompileState
	{
		Successfully,		// 编译成功
		FailedWithErrors,	// 编译错误
	};

	/// <summary>
	/// 编译器错误
	/// </summary>
	/// <param name="compiler">编译过程</param>
	/// <param name="message">错误信息</param>
	/// <param name="">其他参数</param>
	void CompilerError(CompileProcess* compiler, const char* message, ...);

	/// <summary>
	/// 编译器警告
	/// </summary>
	/// <param name="compiler">编译过程</param>
	/// <param name="message">警告信息</param>
	/// <param name="">其他参数</param>
	void CompilerWarning(CompileProcess* compiler, const char* message, ...);

	/// <summary>
	/// 编译文件
	/// </summary>
	/// <param name="fileName">输入文件名</param>
	/// <param name="outFileName">输出文件名</param>
	/// <param name="flags">文件编译方式的标志</param>
	/// <returns>编译器状态</returns>
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags);
}