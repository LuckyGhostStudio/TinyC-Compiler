#include "Compiler.h"
#include "Lexer.h"

#include <stdarg.h>
#include <iostream>

namespace Compiler
{
	void CompilerError(CompileProcess* compiler, const char* message, ...)
	{
		va_list args;	// 参数列表
		va_start(args, message);
		vfprintf(stderr, message, args);
		va_end(args);

		fprintf(stderr, " On Line %i, Column %i in File %s \n", compiler->m_CharPos.Line, compiler->m_CharPos.Column, compiler->m_CharPos.FileName.c_str());
	
		exit(-1);
	}

	void CompilerWarning(CompileProcess* compiler, const char* message, ...)
	{
		va_list args;	// 参数列表
		va_start(args, message);
		vfprintf(stderr, message, args);
		va_end(args);

		fprintf(stderr, " On Line %i, Column %i in File %s \n", compiler->m_CharPos.Line, compiler->m_CharPos.Column, compiler->m_CharPos.FileName.c_str());
	}

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

		//// TODO: test...
		//char c;
		//while ((c = lexer->NextChar()) != EOF) {
		//	std::cout << c;
		//}

		// 词法分析失败
		if (lexer->LexicalAnalysis() != LexicalAnalysisState::Successfully) {
			return CompileState::FailedWithErrors;
		}

		/*for (int i = 0; i < lexer->GetTokens().size(); i++) {
			std::cout << lexer->GetTokens()[i]->m_StringValue << std::endl;
		}*/

		// TODO: 语法分析

		// TODO: 语义解析

		// TODO: 中间代码生成

		// TODO: 目标代码生成...

		return CompileState::Successfully;	// 编译完成
	}
}