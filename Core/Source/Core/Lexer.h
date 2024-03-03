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
		std::vector<Token*> m_Tokens;					// Token序列
		void* m_PrivateData;							// Lexer不能理解的私人数据，使用Lexer的人能够理解
		Token m_TempToken;
		std::string m_TokenBuffer;
	public:
		CharPos m_CharPos;								// 字符位置信息
		CompileProcess* m_Compiler;						// 编译器

		int m_CurrentExpressionCount = 0;				// 当前表达式个数：((50)) 此表达式数有2个
		std::string m_ParenthesesBuffer;				// 括号缓冲

		Lexer(CompileProcess* compiler, void* privateData);

		// TODO: TokenBuildForString

		inline void* GetPrivateData() { return m_PrivateData; }
		inline std::vector<Token*>& GetTokens() { return m_Tokens; }

		inline void ClearTokens() { m_Tokens.clear(); }

		/// <summary>
		/// 返回当前Token序列最后一个Token
		/// </summary>
		/// <returns></returns>
		inline Token* LastToken() { return !m_Tokens.empty() ? m_Tokens.back() : nullptr; }

		/// <summary>
		/// 弹出最后一个Token
		/// </summary>
		inline void PopToken() { m_Tokens.pop_back(); }

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
		void PushChar(/*char c*/);

		/// <summary>
		/// 检查 NextChar 是否为 c
		/// </summary>
		/// <param name="c"></param>
		/// <returns></returns>
		char AssertNextChar(char c);

		/// <summary>
		/// 进行词法分析
		/// </summary>
		/// <returns>词法分析状态</returns>
		LexicalAnalysisState LexicalAnalysis();

		/// <summary>
		/// 创建 Token
		/// </summary>
		/// <param name="token">源token</param>
		/// <returns>目的token</returns>
		Token* CreateToken(Token* token);

		/// <summary>
		/// 处理空白字符并返回下一个Token
		/// </summary>
		/// <returns>下一个Token</returns>
		Token* HandleWhiteSpace();

		/// <summary>
		/// 读取 Number 字符串
		/// </summary>
		/// <returns>Number字符串</returns>
		const char* ReadNumberStr();

		/// <summary>
		/// 读取16进制字符串
		/// </summary>
		/// <returns>16进制字符串</returns>
		const char* ReadHexNumberStr();

		/// <summary>
		/// 读取 long long 无符号 Number
		/// </summary>
		/// <returns>long long Number</returns>
		unsigned long long ReadNumber();

		/// <summary>
		/// 根据数字标识字符获取数值类型
		/// </summary>
		/// <param name="c">数值标识字符</param>
		/// <returns>数值类型</returns>
		NumberType GetNumberType(char c);

		/// <summary>
		/// 获取 Number Token 值
		/// </summary>
		/// <param name="number">long 值</param>
		/// <returns>long Number Token</returns>
		Token* GetNumberTokenForValue(unsigned long number);

		/// <summary>
		/// 获取 Number Token
		/// </summary>
		/// <returns>Number Token</returns>
		Token* GetNumberToken();

		/// <summary>
		/// 获取 16进制数 Token
		/// </summary>
		/// <returns>16进制数 Token</returns>
		Token* GetNumberHexadecimalToken();

		/// <summary>
		/// 验证二进制字符串
		/// </summary>
		/// <param name="str">字符串</param>
		void ValidateBinaryString(const char* str);

		/// <summary>
		/// 获取 2进制数 Token
		/// </summary>
		/// <returns>2进制数 Token</returns>
		Token* GetNumberBinaryToken();

		/// <summary>
		/// 获取特殊数字 Token
		/// </summary>
		/// <returns>16 和 2进制数 Token</returns>
		Token* GetSpecialNumberToken();

		/// <summary>
		/// 获取 字符串 Token
		/// </summary>
		/// <param name="startDelim">前分隔符</param>
		/// <param name="endDelim">后分隔符</param>
		/// <returns>字符串 Token</returns>
		Token* GetStringToken(char startDelim, char endDelim);

		/// <summary>
		/// 读取 运算符
		/// </summary>
		/// <returns>运算符字符串</returns>
		const char* ReadOperator();

		/// <summary>
		/// 新建表达式
		/// </summary>
		void NewExpression();

		/// <summary>
		/// 结束表达式
		/// </summary>
		void FinishExpression();

		/// <summary>
		/// 是否在表达式中
		/// </summary>
		/// <returns></returns>
		bool IsInExpression();

		/// <summary>
		/// 获取 operator或string Token
		/// </summary>
		/// <returns></returns>
		Token* GetOperatorOrStringToken();

		/// <summary>
		/// 获取 单行注释 Token
		/// </summary>
		/// <returns>单行注释 Token</returns>
		Token* GetOneLineCommentToken();

		/// <summary>
		/// 获取 多行注释 Token
		/// </summary>
		/// <returns>多行注释 Token</returns>
		Token* GetMultiLineCommentToken();

		/// <summary>
		/// 处理注释：区分注释和除号运算符
		/// </summary>
		/// <returns></returns>
		Token* HandleComment();

		/// <summary>
		/// 获取 symbol Token
		/// </summary>
		/// <returns>symbol Token</returns>
		Token* GetSymbolToken();

		/// <summary>
		/// 获取 Identifier 或 Keyword Token
		/// </summary>
		/// <returns>Identifier 或 Keyword Token</returns>
		Token* GetIdentifierOrKeyword();

		/// <summary>
		/// 获取特殊 Token: 标识符或关键字或其他特殊 Token
		/// </summary>
		/// <returns></returns>
		Token* ReadSpecialToken();

		/// <summary>
		/// 获取 NewLine Token（新的一行\n）
		/// </summary>
		/// <returns>NewLine Token</returns>
		Token* GetNewLineToken();

		/// <summary>
		/// 获取 '' Token
		/// </summary>
		/// <returns>'' Token</returns>
		Token* GetQuoteToken();

		/// <summary>
		/// 获取下一个Token
		/// </summary>
		/// <returns>Token</returns>
		Token* GetNextToken();
	};
}