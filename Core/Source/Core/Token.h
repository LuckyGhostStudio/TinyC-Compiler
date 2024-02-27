#pragma once

#include <string>

namespace Compiler
{
	/// <summary>
	/// Token类型
	/// </summary>
	enum class TokenType
	{
		Identifier,	// 标识符
		Keyword,	// 关键字	int float ...
		Operator,	// 运算符	+ - * / ...
		Symbol,		// 符号		; : " ...
		Number,		// 数字		0 1 2 3 ...
		String,		// 字符串	"Hello"
		Comment,	// 注释		// ... /* ... */
		NewLine		// 新行
	};

	class Token
	{
	public:
		/* Token 类型 */
		TokenType m_Type;

		/* 标志 */
		int m_Flags;

		/* Token 位置 */
		CharPos m_Pos;

		/* Token 值 */
		union
		{
			char m_CharValue;						// char 类型值
			const char* m_StringValue;				// string 类型值
			unsigned int m_IntNumber;				// int 类型值
			unsigned long m_LongNumber;				// long 类型值
			unsigned long long m_LonglongNumber;	// longlong 类型值
			void* m_Any;							// 其他类型值
		};

		/* 当前 Token 和下一个 Token 之间是否存在空白（空格或别的空白符） */
		bool m_WhiteSpace;

		/* 括号之间的字符串值（调试用）：(10+2+3) */
		std::string m_BetweenBrackets;

		Token() {}
		Token(TokenType type, unsigned long long number) : m_Type(type), m_LonglongNumber(number) {}
		Token(TokenType type, const char* str) : m_Type(type), m_StringValue(str)
		{
			char* temp = new char[strlen(str) + 1];
			strcpy(temp, str);
			m_StringValue = temp;
		}

		/// <summary>
		/// 是否为关键字
		/// </summary>
		/// <param name="value">token 值</param>
		/// <returns></returns>
		bool IsKeyword(const char* value)
		{
			return m_Type == TokenType::Keyword && m_StringValue == value;
		}
	};
}