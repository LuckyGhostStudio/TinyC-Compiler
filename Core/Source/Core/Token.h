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

	/// <summary>
	/// ��ֵ����
	/// </summary>
	enum class NumberType
	{
		Normal,	//û�н�β��ʶ����ֵ����
		Long,
		Float,
		Double
	};

	class Token
	{
	public:
		/* Token ���� */
		TokenType m_Type;

		/* ��־ */
		int m_Flags;

		/* Token λ�� */
		CharPos m_Pos;

		/* Token ֵ */
		union
		{
			char m_CharValue;						// char ����ֵ
			const char* m_StringValue;				// string ����ֵ
			unsigned int m_IntNumber;				// int ����ֵ
			unsigned long m_LongNumber;				// long ����ֵ
			unsigned long long m_LongLongNumber;	// longlong ����ֵ
			void* m_Any;							// ��������ֵ
		};

		/* ��ֵ */
		struct Number
		{
			NumberType Type;	// ��ֵ��������
		} m_Number;

		/* ��ǰ Token ����һ�� Token ֮���Ƿ���ڿհף��ո���Ŀհ׷��� */
		bool m_WhiteSpace;

		/* ����֮����ַ���ֵ�������ã���(10+2+3) */
		std::string m_BetweenBrackets;

		Token() {}
		Token(TokenType type) : m_Type(type) {}
		Token(TokenType type, char c) : m_Type(type), m_CharValue(c) {}
		Token(TokenType type, unsigned long long value) : m_Type(type), m_LongLongNumber(value) {}
		Token(TokenType type, unsigned long long value, NumberType numberType) : m_Type(type), m_LongLongNumber(value)
		{
			m_Number.Type = numberType;
		}

		Token(TokenType type, const char* str) : m_Type(type), m_StringValue(str)
		{
			char* temp = new char[strlen(str) + 1];
			strcpy(temp, str);
			m_StringValue = temp;
		}

		/// <summary>
		/// �Ƿ�Ϊ�ؼ���
		/// </summary>
		/// <param name="value">token ֵ</param>
		/// <returns></returns>
		bool IsKeyword(const char* value)
		{
			return m_Type == TokenType::Keyword && m_StringValue == value;
		}
	};
}