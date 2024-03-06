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
	/// 编译过程：存储编译过程中的数据
	/// </summary>
	class CompileProcess
	{
	public:
		int m_Flags;		// 文件编译方式的标志

		Pos m_CharPos;	// 字符位置信息

		/// <summary>
		/// 输入文件
		/// </summary>
		struct InputFile
		{
			std::ifstream File;			// 文件输入流 .c文件
			std::string AbsolutePath;	// 绝对路径
		} m_CFile;

		std::vector<Token*> m_Tokens;	// 来自 Lexer 的 Token序列

		std::vector<Node*> m_Nodes;		// Node 序列
		std::vector<Node*> m_NodeTrees;	// AST 的实际根

		std::ofstream m_OutFile;		// 文件输出流

		/// <summary>
		/// 编译过程
		/// </summary>
		/// <param name="fileName">输入文件名</param>
		/// <param name="outFileName">输出文件名</param>
		/// <param name="flags">文件编译方式的标志</param>
		CompileProcess(const char* fileName, const char* outFileName, int flags);
	};
}