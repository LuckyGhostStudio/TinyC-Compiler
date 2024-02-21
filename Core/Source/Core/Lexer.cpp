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
		m_Compiler->m_CharPos.Column++;			// ����+1
		char c = static_cast<char>(m_Compiler->m_CFile.File.get());	// ���ļ���������ȡһ���ַ�����ȡ����ַ����ļ���ɾ����
		// ��ǰ�ַ�Ϊ���з�
		if (c == '\n') {
			m_Compiler->m_CharPos.Line++;			// ����+1
			m_Compiler->m_CharPos.Column = 1;		// ��������
		}

		return c;
	}

	char Lexer::PeekChar()
	{
		return static_cast<char>(m_Compiler->m_CFile.File.peek());	// ���ļ���������ȡһ���ַ�
	}
	
	void Lexer::PushChar(char c)
	{
		//ungetc(c, m_Compiler->m_CFile.m_File);		// TODO: �����ַ����ļ�������
	}

	LexicalAnalysisState Lexer::LexicalAnalysis()
	{
		m_CharPos.FileName = m_Compiler->m_CFile.AbsolutePath;

		/* ��ȡ���� Token */
		Token* token = GetNextToken();
		while (token) {
			m_Tokens.push_back(new Token(*token));	// ��ӵ�Tokens�б�

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
		Token* lastToken = LastToken();		// ���һ��token
		if (lastToken) {
			lastToken->m_WhiteSpace = true;
		}

		NextChar();
		return GetNextToken();
	}

	const char* Lexer::ReadNumberStr()
	{
		m_Buffer.clear();

		// ��ȡ������ 0-9 ���ַ���Buffer
		for (char c = PeekChar(); c >= '0' && c <= '9'; c = PeekChar()) {
			m_Buffer += c;
			NextChar();
		}

		return m_Buffer.c_str();
	}

	unsigned long long Lexer::ReadNumber()
	{
		return strtoull(ReadNumberStr(), nullptr, 10);	// תΪ long long ʮ��������
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

		// ��ȡ startDelim �� endDelim ֮����ַ�
		for (char c = NextChar(); c != endDelim && c != EOF; c = NextChar()) {
			if (c == '\\') {
				// TODO: ���� \ ���ţ����� \n
				continue;
			}

			m_Buffer += c;
		}

		return CreateToken(new Token(TokenType::String, m_Buffer.c_str()));
	}

	Token* Lexer::GetNextToken()
	{
		Token* token = nullptr;

		char c = PeekChar();	// ��ȡ��һ���ַ�
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
				token = GetNumberToken();			// ���� Number Token
				break;
			case '"':
				token = GetStringToken('"', '"');	// ���� String Token
				break;
			case ' ':
			case '\t':
				token = HandleWhiteSpace();			// ����հ��ַ�������һ��Token
				break;
			/* �ļ���β */
			case EOF:
				// TODO: ����ɴʷ�����
				break;

			default:
				CompilerError(m_Compiler, "Unexpected token");	// ����������
				break;
		}

		return token;
	}
}