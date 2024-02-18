#pragma once

#include <vector>
#include <string>

#include "CharPos.h"
#include "Token.h"
#include "CompileProcess.h"

namespace Compiler
{
	/// <summary>
	/// �ʷ�����״̬
	/// </summary>
	enum class LexicalAnalysisState
	{
		Successfully,	// �ɹ�
		InputError		// �������
	};

	/// <summary>
	/// �ʷ�������������ʷ����������е���ʱ��Ϣ
	/// </summary>
	class Lexer
	{
	private:
		std::vector<Token> m_Tokens;					// Token����
		void* m_PrivateData;							// Lexer��������˽�����ݣ�ʹ��Lexer�����ܹ����
	public:
		CharPos m_CharPos;								// �ַ�λ����Ϣ
		CompileProcess* m_Compiler;						// ������

		int m_CurrentExpressionCount = 0;				// ��ǰ���ʽ������((50)) �˱��ʽ����2��
		std::vector<std::string> m_ParenthesesBuffer;	// ���Ż���

		Lexer(CompileProcess* compiler, void* privateData);

		inline void* GetPrivateData() { return m_PrivateData; }
		inline std::vector<Token>& GetTokens() { return m_Tokens; }

		void ClearTokens();

		/// <summary>
		/// ������һ���ַ������ļ�����������
		/// </summary>
		/// <returns>��һ���ַ�</returns>
		char NextChar();

		/// <summary>
		/// ������һ���ַ�
		/// </summary>
		/// <returns>��һ���ַ�</returns>
		char PeekChar();

		/// <summary>
		/// �����ַ����ļ�������
		/// </summary>
		/// <param name="c">�ַ�</param>
		void PushChar(char c);

		/// <summary>
		/// ���дʷ�����
		/// </summary>
		/// <returns>�ʷ�����״̬</returns>
		LexicalAnalysisState LexicalAnalysis();
	};
}