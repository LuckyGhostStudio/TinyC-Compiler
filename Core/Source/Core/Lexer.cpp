#include "Lexer.h"
#include "Compiler.h"

#include <cstring>

namespace Compiler
{
	Lexer::Lexer(CompileProcess* compiler, void* privateData)
	{
		m_Compiler = compiler;
		m_PrivateData = privateData;
	}

	char Lexer::NextChar()
	{
		m_Compiler->m_CharPos.Column++;			// 列数+1
		char c = static_cast<char>(m_Compiler->m_CFile.File.get());	// 从文件输入流读取一个字符（读取后该字符从文件流删除）
		// 当前字符为换行符
		if (c == '\n') {
			m_Compiler->m_CharPos.Line++;			// 行数+1
			m_Compiler->m_CharPos.Column = 1;		// 列数重置
		}

		return c;
	}

	char Lexer::PeekChar()
	{
		return static_cast<char>(m_Compiler->m_CFile.File.peek());	// 从文件输入流读取一个字符
	}
	
	void Lexer::PushChar(char c)
	{
		//ungetc(c, m_Compiler->m_CFile.m_File);		// TODO: 回退字符到文件输入流
	}

	LexicalAnalysisState Lexer::LexicalAnalysis()
	{
		m_CharPos.FileName = m_Compiler->m_CFile.AbsolutePath;

		/* 获取所有 Token */
		Token* token = GetNextToken();
		while (token) {
			m_Tokens.push_back(new Token(*token));	// 添加到Tokens列表

			token = GetNextToken();
		}

		return LexicalAnalysisState::Successfully;
	}

	Token* Lexer::CreateToken(Token* token)
	{
		memcpy(&m_TempToken, token, sizeof(class Token));
		m_TempToken.m_Pos = m_CharPos;

		return &m_TempToken;
	}

	Token* Lexer::HandleWhiteSpace()
	{
		Token* lastToken = LastToken();		// 最后一个token
		if (lastToken) {
			lastToken->m_WhiteSpace = true;
		}

		NextChar();
		return GetNextToken();
	}

	const char* Lexer::ReadNumberStr()
	{
		m_Buffer.clear();

		// 读取连续的 0-9 的字符到Buffer
		for (char c = PeekChar(); c >= '0' && c <= '9'; c = PeekChar()) {
			m_Buffer += c;
			NextChar();
		}

		return m_Buffer.c_str();
	}

	unsigned long long Lexer::ReadNumber()
	{
		return strtoull(ReadNumberStr(), nullptr, 10);	// 转为 long long 十进制类型
	}

	Token* Lexer::GetNumberTokenForValue(unsigned long number)
	{
		return CreateToken(new Token(TokenType::Number, number));
	}

	Token* Lexer::GetNumberToken()
	{
		return GetNumberTokenForValue(ReadNumber());
	}

	Token* Lexer::GetStringToken(char startDelim, char endDelim)
	{
		m_Buffer.clear();
		NextChar();

		// 读取 startDelim 和 endDelim 之间的字符
		for (char c = NextChar(); c != endDelim && c != EOF; c = NextChar()) {
			if (c == '\\') {
				// TODO: 处理 \ 符号，例如 \n
				continue;
			}

			m_Buffer += c;
		}

		return CreateToken(new Token(TokenType::String, m_Buffer.c_str()));
	}

	Token* Lexer::GetNextToken()
	{
		Token* token = nullptr;

		char c = PeekChar();	// 读取下一个字符
		switch (c) {
			/* Number */
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				token = GetNumberToken();			// 返回 Number Token
				break;
			case '"':
				token = GetStringToken('"', '"');	// 返回 String Token
				break;
			case ' ':
			case '\t':
				token = HandleWhiteSpace();			// 处理空白字符返回下一个Token
				break;
			/* 文件结尾 */
			case EOF:
				// TODO: 已完成词法分析
				break;

			default:
				CompilerError(m_Compiler, "Unexpected token");	// 编译器错误
				break;
		}

		return token;
	}
}