#include "Compiler.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

#include <stdarg.h>
#include <iostream>

namespace Compiler
{
	void CompilerError(CompileProcess* compiler, const char* message, ...)
	{
		va_list args;	// �����б�
		va_start(args, message);
		vfprintf(stderr, message, args);
		va_end(args);

		fprintf(stderr, " On Line %i, Column %i in File %s \n", compiler->m_CharPos.Line, compiler->m_CharPos.Column, compiler->m_CharPos.FileName.c_str());
	
		exit(-1);
	}

	void CompilerWarning(CompileProcess* compiler, const char* message, ...)
	{
		va_list args;	// �����б�
		va_start(args, message);
		vfprintf(stderr, message, args);
		va_end(args);

		fprintf(stderr, " On Line %i, Column %i in File %s \n", compiler->m_CharPos.Line, compiler->m_CharPos.Column, compiler->m_CharPos.FileName.c_str());
	}

	CompileState CompileFile(const char* fileName, const char* outFileName, int flags)
	{
		CompileProcess* compiler = new CompileProcess(fileName, outFileName, flags);	// �����������
		if (!compiler) {
			return CompileState::FailedWithErrors;
		}

		/* �ʷ����� */
		Lexer* lexer = new Lexer(compiler, nullptr);	// �ʷ�������
		if (!lexer) {
			return CompileState::FailedWithErrors;
		}

		// �ʷ�����ʧ��
		if (lexer->LexicalAnalysis() != LexicalAnalysisState::Successfully) {
			return CompileState::FailedWithErrors;
		}

		compiler->m_Tokens = lexer->GetTokens();	// ��¼�ʷ����������Token����

		/* �﷨���� */
		Parser* parser = new Parser(compiler);	// �﷨������

		// �﷨����ʧ��
		if (parser->Parse() != ParseState::Successfully) {
			return CompileState::FailedWithErrors;
		}

		// TODO: �������

		// TODO: �м��������

		// TODO: Ŀ���������...

		return CompileState::Successfully;	// �������
	}
}