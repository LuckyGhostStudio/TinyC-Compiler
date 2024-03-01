#pragma once

#include <string>
#include <unordered_set>

namespace Utilities
{
	/* 单个运算符 */
	static std::unordered_set<char> singleOperators = {
		'+', '-', '/',	'=', '%', '>', '<', '|', '&', '!', '^', '~',
		'*', '(', '[', ',', '.', '?'
	};

	/* 有效的运算符 */
	static std::unordered_set<std::string> validOperators = {
		"+", "-", "*", "/", "%",
		"&", "|", "!", "^", "&&", "||", "~", ">>", "<<",
		">", "<", ">=", "<=", "!=", "==",
		"++", "--", "=", "+=", "-=", "*=", "/=",
		"(", "[", ",",
		".", "->",
		"...",	// TODO delete
		"?"
	};

	/* 支持的关键字 */
	static std::unordered_set<std::string> keywords = {
		"unsigned", "signed", "char", "short", "int", "long", "float", "double", "void",
		"struct", "union", "static", "typedef", "const",
		"include", "sizeof",
		"if", "else", "while", "for", "do", "break", "continue", "switch", "case", "default", "goto", "return",
		"extern", "restrict",
		"__ignore_typecheck"	// 忽略类型检查
	};

	/// <summary>
	/// 可否被视为单个运算符
	/// </summary>
	/// <param name="op">运算符</param>
	/// <returns></returns>
	static bool TreatedAsOneOperator(char op)
	{
		return op == '(' || op == '[' || op == ',' || op == '.' || op == '*' || op == '?';
	}

	/// <summary>
	/// 是单个运算符
	/// </summary>
	/// <param name="op">运算符</param>
	/// <returns></returns>
	static bool IsSingleOperator(char op)
	{
		return singleOperators.find(op) != singleOperators.end();
	}

	/// <summary>
	/// 运算符是否有效
	/// </summary>
	/// <param name="op">运算符</param>
	/// <returns></returns>
	static bool OperatorValid(const std::string& op)
	{
		return validOperators.find(op) != validOperators.end();
	}

	/// <summary>
	/// 是关键字
	/// </summary>
	/// <param name="str">字符串</param>
	/// <returns></returns>
	static bool IsKeyword(const std::string& str)
	{
		return keywords.find(str) != keywords.end();
	}

	/// <summary>
	/// 返回转义字符
	/// </summary>
	/// <param name="c">字符</param>
	/// <returns>转义字符</returns>
	static char GetEscapedChar(char c)
	{
		char co = 0;

		switch (c)
		{
		case 'n':
			co = '\n';
			break;
		case '\\':
			co = '\\';
			break;
		case 't':
			co = '\t';
			break;
		case '\'':
			co = '\'';
			break;
		}

		return co;
	}

	/// <summary>
	/// 是16进制字符
	/// </summary>
	/// <param name="c">字符</param>
	/// <returns></returns>
	static bool IsHexChar(char c)
	{
		c = tolower(c);
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
	}
}