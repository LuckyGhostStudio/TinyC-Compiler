#pragma once

#include <string>

namespace Compiler
{
	/// <summary>
	/// Token����
	/// </summary>
	enum class TokenType
	{
		Identifier,	// ��ʶ��
		Keyword,	// �ؼ���	int float ...
		Operator,	// �����	+ - * / ...
		Symbol,		// ����		; : " ...
		Number,		// ����		0 1 2 3 ...
		String,		// �ַ���	"Hello"
		Comment,	// ע��		// ... /* ... */
		NewLine		// ����
	};

	class Token
	{
	public:
		/* Token ���� */
		TokenType m_Type;

		/* ��־ */
		int m_Flags;

		/* Token ֵ */
		union
		{
			char m_CharValue;						// char ����ֵ
			const char* m_StringValue;				// string ����ֵ
			unsigned int m_IntNumber;				// int ����ֵ
			unsigned long m_LongNumber;				// long ����ֵ
			unsigned long long m_LonglongNumber;	// longlong ����ֵ
			void* m_Any;							// ��������ֵ
		};

		/* ��ǰ Token ����һ�� Token ֮���Ƿ���ڿհף��ո���Ŀհ׷��� */
		bool m_WhiteSpace;

		/* ����֮����ַ���ֵ�������ã���(10+2+3) */
		std::string m_BetweenBrackets;
	};
}