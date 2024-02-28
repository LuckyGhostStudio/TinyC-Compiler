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
		std::vector<Token*> m_Tokens;					// Token����
		void* m_PrivateData;							// Lexer��������˽�����ݣ�ʹ��Lexer�����ܹ����
		Token m_TempToken;
		std::string m_TokenBuffer;
	public:
		CharPos m_CharPos;								// �ַ�λ����Ϣ
		CompileProcess* m_Compiler;						// ������

		int m_CurrentExpressionCount = 0;				// ��ǰ���ʽ������((50)) �˱��ʽ����2��
		std::string m_ParenthesesBuffer;				// ���Ż���

		Lexer(CompileProcess* compiler, void* privateData);

		inline void* GetPrivateData() { return m_PrivateData; }
		inline std::vector<Token*>& GetTokens() { return m_Tokens; }

		inline void ClearTokens() { m_Tokens.clear(); }

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
		void PushChar(/*char c*/);

		/// <summary>
		/// ���дʷ�����
		/// </summary>
		/// <returns>�ʷ�����״̬</returns>
		LexicalAnalysisState LexicalAnalysis();

		/// <summary>
		/// ���� Token
		/// </summary>
		/// <param name="token">Դtoken</param>
		/// <returns>Ŀ��token</returns>
		Token* CreateToken(Token* token);

		/// <summary>
		/// ���ص�ǰToken�������һ��Token
		/// </summary>
		/// <returns></returns>
		Token* LastToken() { return !m_Tokens.empty() ? m_Tokens.back() : nullptr; }

		/// <summary>
		/// ����հ��ַ���������һ��Token
		/// </summary>
		/// <returns>��һ��Token</returns>
		Token* HandleWhiteSpace();

		/// <summary>
		/// ��ȡ Number �ַ���
		/// </summary>
		/// <returns>Number�ַ���</returns>
		const char* ReadNumberStr();

		/// <summary>
		/// ��ȡ long long �޷��� Number
		/// </summary>
		/// <returns>long long Number</returns>
		unsigned long long ReadNumber();

		/// <summary>
		/// ��ȡ Number Token ֵ
		/// </summary>
		/// <param name="number">long ֵ</param>
		/// <returns>long Number Token</returns>
		Token* GetNumberTokenForValue(unsigned long number);

		/// <summary>
		/// ��ȡ Number Token
		/// </summary>
		/// <returns>Number Token</returns>
		Token* GetNumberToken();

		/// <summary>
		/// ��ȡ �ַ��� Token
		/// </summary>
		/// <param name="startDelim">ǰ�ָ���</param>
		/// <param name="endDelim">��ָ���</param>
		/// <returns>�ַ��� Token</returns>
		Token* GetStringToken(char startDelim, char endDelim);

		/// <summary>
		/// ��ȡ �����
		/// </summary>
		/// <returns>������ַ���</returns>
		const char* ReadOperator();

		/// <summary>
		/// �½����ʽ
		/// </summary>
		void NewExpression();

		/// <summary>
		/// �������ʽ
		/// </summary>
		void FinishExpression();

		/// <summary>
		/// �Ƿ��ڱ��ʽ��
		/// </summary>
		/// <returns></returns>
		bool IsInExpression();

		/// <summary>
		/// ��ȡ operator��string Token
		/// </summary>
		/// <returns></returns>
		Token* GetOperatorOrStringToken();

		/// <summary>
		/// ��ȡ symbol Token
		/// </summary>
		/// <returns>symbol Token</returns>
		Token* GetSymbolToken();

		/// <summary>
		/// ��ȡ Identifier �� Keyword Token
		/// </summary>
		/// <returns>Identifier �� Keyword Token</returns>
		Token* GetIdentifierOrKeyword();

		/// <summary>
		/// ��ȡ���� Token: ��ʶ����ؼ��ֻ��������� Token
		/// </summary>
		/// <returns></returns>
		Token* ReadSpecialToken();

		/// <summary>
		/// ��ȡ��һ��Token
		/// </summary>
		/// <returns>Token</returns>
		Token* GetNextToken();
	};
}