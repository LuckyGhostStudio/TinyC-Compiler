#pragma once

#include <string>

namespace Compiler
{
	/// <summary>
	/// 位置信息
	/// </summary>
	struct Pos
	{
		int Line = 1;			// 行号
		int Column = 1;			// 列号
		std::string FileName;	// 文件名
	};
}