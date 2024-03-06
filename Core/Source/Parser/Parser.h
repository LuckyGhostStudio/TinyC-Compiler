#pragma once

#include "Core/CompileProcess.h"

namespace Compiler
{
	/// <summary>
	/// �﷨����״̬
	/// </summary>
	enum class ParseState
	{
		Successfully,	// �ɹ�
		GeneralError	// ͨ�ô���
	};

	/// <summary>
	/// �﷨������
	/// </summary>
	class Parser
	{
	private:
		CompileProcess* m_Compiler;		// ������
	private:
		int Next();
	public:
		Parser(CompileProcess* compiler);

		ParseState Parse();
	};
}