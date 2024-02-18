#pragma once

#include <string>

namespace Compiler
{
	/// <summary>
	/// 字符位置信息
	/// </summary>
	struct CharPos
	{
		int Line = 1;			// 行号
		int Column = 1;			// 列号
		std::string FileName;	// 文件名
	};
}