#pragma once

#include "Pos.h"

namespace Compiler
{
	/// <summary>
	/// 结点类型
	/// </summary>
	enum class NodeType
	{
		Expression,				// 表达式
		ExpressionParentheses,	// 括号表达式

		Number,
		Identifier,
		String,
		Variable,
		VariableList,
		Function,
		Body,

		/* 语句 */
		StatementReturn,	// return
		StatementIf,		// if
		StatementElse,		// else
		StatementWhile,		// while
		StatementDoWhile,	// do while
		StatementFor,		// for
		StatementBreak,		// break
		StatementContinue,	// continue
		StatementSwitch,	// switch
		StatementCase,		// case
		StatementDefault,	// default
		StatementGoto,		// goto

		Unary,
		Tenary,
		Label,
		Struct,
		Union,
		Bracket,
		Cast,
		Blank		// 空白
	};

	/// <summary>
	/// 抽象语法树结点
	/// </summary>
	class Node
	{
		NodeType m_Type;	// 结点类型
		int m_Flags;

		Pos m_Pos;			// 结点位置

		struct NodeBlinded
		{
			Node* Owner;	// Body 结点
			Node* Function;	// 该结点所在的函数
		} m_Blinded;

		/* 结点字面值 */
		union
		{
			char m_CharValue;						// char 类型值
			const char* m_StringValue;				// string 类型值
			unsigned int m_IntNumber;				// int 类型值
			unsigned long m_LongNumber;				// long 类型值
			unsigned long long m_LongLongNumber;	// longlong 类型值
		};
	};
}