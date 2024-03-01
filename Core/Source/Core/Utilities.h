#pragma once

#include <string>
#include <unordered_set>

namespace Utilities
{
	/* ��������� */
	static std::unordered_set<char> singleOperators = {
		'+', '-', '/',	'=', '%', '>', '<', '|', '&', '!', '^', '~',
		'*', '(', '[', ',', '.', '?'
	};

	/* ��Ч������� */
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

	/* ֧�ֵĹؼ��� */
	static std::unordered_set<std::string> keywords = {
		"unsigned", "signed", "char", "short", "int", "long", "float", "double", "void",
		"struct", "union", "static", "typedef", "const",
		"include", "sizeof",
		"if", "else", "while", "for", "do", "break", "continue", "switch", "case", "default", "goto", "return",
		"extern", "restrict",
		"__ignore_typecheck"	// �������ͼ��
	};

	/// <summary>
	/// �ɷ���Ϊ���������
	/// </summary>
	/// <param name="op">�����</param>
	/// <returns></returns>
	static bool TreatedAsOneOperator(char op)
	{
		return op == '(' || op == '[' || op == ',' || op == '.' || op == '*' || op == '?';
	}

	/// <summary>
	/// �ǵ��������
	/// </summary>
	/// <param name="op">�����</param>
	/// <returns></returns>
	static bool IsSingleOperator(char op)
	{
		return singleOperators.find(op) != singleOperators.end();
	}

	/// <summary>
	/// ������Ƿ���Ч
	/// </summary>
	/// <param name="op">�����</param>
	/// <returns></returns>
	static bool OperatorValid(const std::string& op)
	{
		return validOperators.find(op) != validOperators.end();
	}

	/// <summary>
	/// �ǹؼ���
	/// </summary>
	/// <param name="str">�ַ���</param>
	/// <returns></returns>
	static bool IsKeyword(const std::string& str)
	{
		return keywords.find(str) != keywords.end();
	}

	/// <summary>
	/// ����ת���ַ�
	/// </summary>
	/// <param name="c">�ַ�</param>
	/// <returns>ת���ַ�</returns>
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
	/// ��16�����ַ�
	/// </summary>
	/// <param name="c">�ַ�</param>
	/// <returns></returns>
	static bool IsHexChar(char c)
	{
		c = tolower(c);
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
	}
}