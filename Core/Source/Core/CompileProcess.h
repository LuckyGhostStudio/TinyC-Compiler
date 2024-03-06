#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Pos.h"
#include "Token.h"
#include "Node.h"

namespace Compiler
{
	/// <summary>
	/// ������̣��洢��������е�����
	/// </summary>
	class CompileProcess
	{
	public:
		int m_Flags;		// �ļ����뷽ʽ�ı�־

		Pos m_CharPos;	// �ַ�λ����Ϣ

		/// <summary>
		/// �����ļ�
		/// </summary>
		struct InputFile
		{
			std::ifstream File;			// �ļ������� .c�ļ�
			std::string AbsolutePath;	// ����·��
		} m_CFile;

		std::vector<Token*> m_Tokens;	// ���� Lexer �� Token����

		std::vector<Node*> m_Nodes;		// Node ����
		std::vector<Node*> m_NodeTrees;	// AST ��ʵ�ʸ�

		std::ofstream m_OutFile;		// �ļ������

		/// <summary>
		/// �������
		/// </summary>
		/// <param name="fileName">�����ļ���</param>
		/// <param name="outFileName">����ļ���</param>
		/// <param name="flags">�ļ����뷽ʽ�ı�־</param>
		CompileProcess(const char* fileName, const char* outFileName, int flags);
	};
}