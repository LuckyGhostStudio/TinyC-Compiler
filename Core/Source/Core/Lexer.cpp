#include "Lexer.h"

namespace Compiler
{
	Lexer::Lexer(CompileProcess* compiler, void* privateData)
	{
		m_Compiler = compiler;
		m_PrivateData = privateData;
	}

	void Lexer::ClearTokens()
	{
		m_Tokens.clear();
	}

	char Lexer::NextChar()
	{
		CompileProcess* compiler = m_Compiler;

		compiler->m_CharPos.Column++;			// 列数+1
		char c = static_cast<char>(compiler->m_CFile.File.get());	// 从文件输入流读取一个字符（读取后该字符从文件流删除）
		// 当前字符为换行符
		if (c == '\n') {
			compiler->m_CharPos.Line++;			// 行数+1
			compiler->m_CharPos.Column = 1;		// 列数重置
		}

		return c;
	}

	char Lexer::PeekChar()
	{
		CompileProcess* compiler = m_Compiler;

		return static_cast<char>(compiler->m_CFile.File.peek());	// 从文件输入流读取一个字符
	}
	
	void Lexer::PushChar(char c)
	{
		CompileProcess* compiler = m_Compiler;

		//ungetc(c, compiler->m_CFile.m_File);		// TODO: 回退字符到文件输入流
	}

	LexicalAnalysisState Lexer::LexicalAnalysis()
	{
		// TODO: 进行词法分析

		return LexicalAnalysisState::Successfully;
	}
}