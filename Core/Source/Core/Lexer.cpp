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

		compiler->m_CharPos.Column++;			// ����+1
		char c = static_cast<char>(compiler->m_CFile.File.get());	// ���ļ���������ȡһ���ַ�����ȡ����ַ����ļ���ɾ����
		// ��ǰ�ַ�Ϊ���з�
		if (c == '\n') {
			compiler->m_CharPos.Line++;			// ����+1
			compiler->m_CharPos.Column = 1;		// ��������
		}

		return c;
	}

	char Lexer::PeekChar()
	{
		CompileProcess* compiler = m_Compiler;

		return static_cast<char>(compiler->m_CFile.File.peek());	// ���ļ���������ȡһ���ַ�
	}
	
	void Lexer::PushChar(char c)
	{
		CompileProcess* compiler = m_Compiler;

		//ungetc(c, compiler->m_CFile.m_File);		// TODO: �����ַ����ļ�������
	}

	LexicalAnalysisState Lexer::LexicalAnalysis()
	{
		// TODO: ���дʷ�����

		return LexicalAnalysisState::Successfully;
	}
}