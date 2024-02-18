#include "Compiler.h"
#include <iostream>
namespace Compiler
{
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags)
	{
		CompileProcess* compiler = new CompileProcess(fileName, outFileName, flags);	// 创建编译过程
		if (!compiler) {
			return CompileState::FailedWithErrors;
		}

		/* TODO: 词法分析 */
		Lexer* lexer = new Lexer(compiler, nullptr);	// 词法分析器
		if (!lexer) {
			return CompileState::FailedWithErrors;
		}

		// TODO: test...
		char c;
		while ((c = lexer->NextChar()) != EOF) {
			std::cout << c;
		}

		// 词法分析失败
		if (lexer->LexicalAnalysis() != LexicalAnalysisState::Successfully) {
			return CompileState::FailedWithErrors;
		}

		// TODO: 语法分析

		// TODO: 语义解析

		// TODO: 中间代码生成

		// TODO: 目标代码生成...

		return CompileState::Successfully;	// 编译完成
	}
}