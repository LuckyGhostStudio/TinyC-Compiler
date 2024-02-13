#pragma once

#include <cstdio>

namespace Compiler
{
	struct Position
	{
		int line;
		int column;
		const char* fileName;
	};

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

	struct Token
	{
		TokenType type;
		int flags;

		/* token 值 */
		union
		{
			char charValue;						// char 类型值
			const char* stringValue;			// string 类型值
			unsigned int intNumber;				// int 类型值
			unsigned long longNumber;			// long 类型值
			unsigned long long longlongNumber;	// longlong 类型值
			void* any;							// 其他类型值
		};

		bool whiteSpace;				// 当前token和下一个token之间是否存在空白（空格或别的空白符）

		const char* between_brackets;	// 括号之间的字符串值（调试用）
	};

	enum class CompileState
	{
		FileCompiledOK,		// 文件编译成功
		FailedWithErrors,	// 错误
	};

	/// <summary>
	/// 编译过程
	/// </summary>
	struct CompileProcess
	{
		int flags;	// 文件编译方式的标志

		/// <summary>
		/// 输入文件编译过程
		/// </summary>
		struct InputFileCompileProcess
		{
			FILE* file;					// 文件
			const char* absolutePath;	// 绝对路径
		} cFile;

		FILE* outFile;	// 输出文件
	};

	/// <summary>
	/// 编译文件
	/// </summary>
	/// <param name="fileName">输入文件名</param>
	/// <param name="outFileName">输出文件名</param>
	/// <param name="flags">文件编译方式的标志</param>
	/// <returns>编译器状态</returns>
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags);

	/// <summary>
	/// 创建编译过程
	/// </summary>
	/// <param name="fileName">输入文件名</param>
	/// <param name="outFileName">输出文件名</param>
	/// <param name="flags">文件编译方式的标志</param>
	/// <returns>编译过程</returns>
	CompileProcess* CreateCompileProcess(const char* fileName, const char* outFileName, int flags);
}