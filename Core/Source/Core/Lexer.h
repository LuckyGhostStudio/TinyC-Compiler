#pragma once

#include <vector>
#include <string>

#include "CharPos.h"
#include "Token.h"
#include "CompileProcess.h"

namespace Compiler
{
	/// <summary>
	/// 词法分析状态
	/// </summary>
	enum class LexicalAnalysisState
	{
		Successfully,	// 成功
		InputError		// 输入错误
	};

	/// <summary>
	/// 词法分析器：保存词法分析过程中的临时信息
	/// </summary>
	class Lexer
	{
	private:
		std::vector<Token> m_Tokens;					// Token序列
		void* m_PrivateData;							// Lexer不能理解的私人数据，使用Lexer的人能够理解
	public:
		CharPos m_CharPos;								// 字符位置信息
		CompileProcess* m_Compiler;						// 编译器

		int m_CurrentExpressionCount = 0;				// 当前表达式个数：((50)) 此表达式数有2个
		std::vector<std::string> m_ParenthesesBuffer;	// 括号缓冲

		Lexer(CompileProcess* compiler, void* privateData);

		inline void* GetPrivateData() { return m_PrivateData; }
		inline std::vector<Token>& GetTokens() { return m_Tokens; }

		void ClearTokens();

		/// <summary>
		/// 返回下一个字符并从文件输入流弹出
		/// </summary>
		/// <returns>下一个字符</returns>
		char NextChar();

		/// <summary>
		/// 返回下一个字符
		/// </summary>
		/// <returns>下一个字符</returns>
		char PeekChar();

		/// <summary>
		/// 回退字符到文件输入流
		/// </summary>
		/// <param name="c">字符</param>
		void PushChar(char c);

		/// <summary>
		/// 进行词法分析
		/// </summary>
		/// <returns>词法分析状态</returns>
		LexicalAnalysisState LexicalAnalysis();
	};
}