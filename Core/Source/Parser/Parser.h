#pragma once

#include "Core/CompileProcess.h"

namespace Compiler
{
	/// <summary>
	/// 语法分析状态
	/// </summary>
	enum class ParseState
	{
		Successfully,	// 成功
		GeneralError	// 通用错误
	};

	/// <summary>
	/// 语法分析器
	/// </summary>
	class Parser
	{
	private:
		CompileProcess* m_Compiler;		// 编译器
	private:
		int Next();
	public:
		Parser(CompileProcess* compiler);

		ParseState Parse();
	};
}