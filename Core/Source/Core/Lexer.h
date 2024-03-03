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

		// TODO: TokenBuildForString

		inline void* GetPrivateData() { return m_PrivateData; }
		inline std::vector<Token*>& GetTokens() { return m_Tokens; }

		inline void ClearTokens() { m_Tokens.clear(); }

		/// <summary>
		/// ���ص�ǰToken�������һ��Token
		/// </summary>
		/// <returns></returns>
		inline Token* LastToken() { return !m_Tokens.empty() ? m_Tokens.back() : nullptr; }

		/// <summary>
		/// �������һ��Token
		/// </summary>
		inline void PopToken() { m_Tokens.pop_back(); }

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
		/// ��� NextChar �Ƿ�Ϊ c
		/// </summary>
		/// <param name="c"></param>
		/// <returns></returns>
		char AssertNextChar(char c);

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
		/// ��ȡ16�����ַ���
		/// </summary>
		/// <returns>16�����ַ���</returns>
		const char* ReadHexNumberStr();

		/// <summary>
		/// ��ȡ long long �޷��� Number
		/// </summary>
		/// <returns>long long Number</returns>
		unsigned long long ReadNumber();

		/// <summary>
		/// �������ֱ�ʶ�ַ���ȡ��ֵ����
		/// </summary>
		/// <param name="c">��ֵ��ʶ�ַ�</param>
		/// <returns>��ֵ����</returns>
		NumberType GetNumberType(char c);

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
		/// ��ȡ 16������ Token
		/// </summary>
		/// <returns>16������ Token</returns>
		Token* GetNumberHexadecimalToken();

		/// <summary>
		/// ��֤�������ַ���
		/// </summary>
		/// <param name="str">�ַ���</param>
		void ValidateBinaryString(const char* str);

		/// <summary>
		/// ��ȡ 2������ Token
		/// </summary>
		/// <returns>2������ Token</returns>
		Token* GetNumberBinaryToken();

		/// <summary>
		/// ��ȡ�������� Token
		/// </summary>
		/// <returns>16 �� 2������ Token</returns>
		Token* GetSpecialNumberToken();

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
		/// ��ȡ ����ע�� Token
		/// </summary>
		/// <returns>����ע�� Token</returns>
		Token* GetOneLineCommentToken();

		/// <summary>
		/// ��ȡ ����ע�� Token
		/// </summary>
		/// <returns>����ע�� Token</returns>
		Token* GetMultiLineCommentToken();

		/// <summary>
		/// ����ע�ͣ�����ע�ͺͳ��������
		/// </summary>
		/// <returns></returns>
		Token* HandleComment();

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
		/// ��ȡ NewLine Token���µ�һ��\n��
		/// </summary>
		/// <returns>NewLine Token</returns>
		Token* GetNewLineToken();

		/// <summary>
		/// ��ȡ '' Token
		/// </summary>
		/// <returns>'' Token</returns>
		Token* GetQuoteToken();

		/// <summary>
		/// ��ȡ��һ��Token
		/// </summary>
		/// <returns>Token</returns>
		Token* GetNextToken();
	};
}