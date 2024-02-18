#pragma once

#include "CharPos.h"

#include <fstream>
#include <string>

namespace Compiler
{
	/// <summary>
	/// 编译过程
	/// </summary>
	class CompileProcess
	{
	public:
		int m_Flags;		// 文件编译方式的标志

		CharPos m_CharPos;	// 字符位置信息

		/// <summary>
		/// 输入文件
		/// </summary>
		struct InputFile
		{
			std::ifstream File;			// 文件输入流
			std::string AbsolutePath;	// 绝对路径
		} m_CFile;

		std::ofstream m_OutFile;			// 文件输出流

		/// <summary>
		/// 编译过程
		/// </summary>
		/// <param name="fileName">输入文件名</param>
		/// <param name="outFileName">输出文件名</param>
		/// <param name="flags">文件编译方式的标志</param>
		CompileProcess(const char* fileName, const char* outFileName, int flags);
	};
}