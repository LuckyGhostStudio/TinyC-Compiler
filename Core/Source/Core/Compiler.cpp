#include "Compiler.h"
#include <iostream>
namespace Compiler
{
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

		// TODO: test...
		char c;
		while ((c = lexer->NextChar()) != EOF) {
			std::cout << c;
		}

		// �ʷ�����ʧ��
		if (lexer->LexicalAnalysis() != LexicalAnalysisState::Successfully) {
			return CompileState::FailedWithErrors;
		}

		// TODO: �﷨����

		// TODO: �������

		// TODO: �м��������

		// TODO: Ŀ���������...

		return CompileState::Successfully;	// �������
	}
}