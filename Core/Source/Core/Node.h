#pragma once

#include "Pos.h"

namespace Compiler
{
	/// <summary>
	/// �������
	/// </summary>
	enum class NodeType
	{
		Expression,				// ���ʽ
		ExpressionParentheses,	// ���ű��ʽ

		Number,
		Identifier,
		String,
		Variable,
		VariableList,
		Function,
		Body,

		/* ��� */
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
		Blank		// �հ�
	};

	/// <summary>
	/// �����﷨�����
	/// </summary>
	class Node
	{
		NodeType m_Type;	// �������
		int m_Flags;

		Pos m_Pos;			// ���λ��

		struct NodeBlinded
		{
			Node* Owner;	// Body ���
			Node* Function;	// �ý�����ڵĺ���
		} m_Blinded;

		/* �������ֵ */
		union
		{
			char m_CharValue;						// char ����ֵ
			const char* m_StringValue;				// string ����ֵ
			unsigned int m_IntNumber;				// int ����ֵ
			unsigned long m_LongNumber;				// long ����ֵ
			unsigned long long m_LongLongNumber;	// longlong ����ֵ
		};
	};
}