#include "Compiler.h"
#include "Lexer.h"

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

		/* TODO: �ʷ����� */
		Lexer* lexer = new Lexer(compiler, nullptr);	// �ʷ�������
		if (!lexer) {
			return CompileState::FailedWithErrors;
		}

		//// TODO: test...
		//char c;
		//while ((c = lexer->NextChar()) != EOF) {
		//	std::cout << c;
		//}

		// �ʷ�����ʧ��
		if (lexer->LexicalAnalysis() != LexicalAnalysisState::Successfully) {
			return CompileState::FailedWithErrors;
		}

		/*for (int i = 0; i < lexer->GetTokens().size(); i++) {
			std::cout << lexer->GetTokens()[i]->m_StringValue << std::endl;
		}*/

		// TODO: �﷨����

		// TODO: �������

		// TODO: �м��������

		// TODO: Ŀ���������...

		return CompileState::Successfully;	// �������
	}
}