#include "Lexer.h"
#include "Compiler.h"

#include <cstring>
#include <ctype.h>
#include <unordered_set>
#include <cassert>

namespace Compiler
{
	/* 单个运算符 */
	static std::unordered_set<char> singleOperators = {
		'+', '-', '/',	'=', '%', '>', '<', '|', '&', '!', '^', '~',
		'*', '(', '[', ',', '.', '?'
	};

	/* 有效的运算符 */
	static std::unordered_set<std::string> validOperators = {
		"+", "-", "*", "/", "%",
		"&", "|", "!", "^", "&&", "||", "~", ">>", "<<",
		">", "<", ">=", "<=", "!=", "==",
		"++", "--", "=", "+=", "-=", "*=", "/=",
		"(", "[", ",",
		".", "->",
		"...",	// TODO delete
		"?"
	};

	/* 支持的关键字 */
	static std::unordered_set<std::string> keywords = {
		"unsigned", "signed", "char", "short", "int", "long", "float", "double", "void",
		"struct", "union", "static", "typedef", "const",
		"include", "sizeof",
		"if", "else", "while", "for", "do", "break", "continue", "switch", "case", "default", "goto", "return",
		"extern", "restrict",
		"__ignore_typecheck"	// 忽略类型检查
	};

	Lexer::Lexer(CompileProcess* compiler, void* privateData)
	{
		m_Compiler = compiler;
		m_PrivateData = privateData;
	}

	char Lexer::NextChar()
	{
		m_Compiler->m_CharPos.Column++;			// 列数+1
		m_CharPos.Column++;
		char c = static_cast<char>(m_Compiler->m_CFile.File.get());	// 从文件输入流读取一个字符（读取后该字符从文件流删除）
		// 当前字符为换行符
		if (c == '\n') {
			m_Compiler->m_CharPos.Line++;			// 行数+1
			m_CharPos.Line++;
			m_Compiler->m_CharPos.Column = 1;		// 列数重置
			m_CharPos.Column = 1;
		}

		return c;
	}

	char Lexer::PeekChar()
	{
		return static_cast<char>(m_Compiler->m_CFile.File.peek());	// 从文件输入流读取一个字符
	}

	void Lexer::PushChar(/*char c*/)
	{
		// ungetc(c, m_Compiler->m_CFile.m_File);		// TODO: 回退字符到文件输入流
		m_Compiler->m_CFile.File.unget();
	}

	char Lexer::AssertNextChar(char c)
	{
		char nextC = NextChar();
		assert(c == nextC);

		return nextC;
	}

	LexicalAnalysisState Lexer::LexicalAnalysis()
	{
		m_CharPos.FileName = m_Compiler->m_CFile.AbsolutePath;

		/* 获取所有 Token */
		Token* token = GetNextToken();
		while (token) {
			m_Tokens.push_back(new Token(*token));	// 添加到Tokens列表

			token = GetNextToken();
		}

		return LexicalAnalysisState::Successfully;
	}

	Token* Lexer::CreateToken(Token* token)
	{
		memcpy(&m_TempToken, token, sizeof(class Token));
		m_TempToken.m_Pos = m_CharPos;

		return &m_TempToken;
	}

	Token* Lexer::HandleWhiteSpace()
	{
		Token* lastToken = LastToken();		// 最后一个token
		if (lastToken) {
			lastToken->m_WhiteSpace = true;
		}

		NextChar();
		return GetNextToken();
	}

	const char* Lexer::ReadNumberStr()
	{
		m_TokenBuffer.clear();

		// 读取连续的 0-9 的字符到Buffer
		for (char c = PeekChar(); c >= '0' && c <= '9'; c = PeekChar()) {
			m_TokenBuffer += c;
			NextChar();
		}

		return m_TokenBuffer.c_str();
	}

	unsigned long long Lexer::ReadNumber()
	{
		return strtoull(ReadNumberStr(), nullptr, 10);	// 转为 long long 十进制类型
	}

	Token* Lexer::GetNumberTokenForValue(unsigned long number)
	{
		return CreateToken(new Token(TokenType::Number, static_cast<unsigned long long>(number)));
	}

	Token* Lexer::GetNumberToken()
	{
		return GetNumberTokenForValue(ReadNumber());
	}

	Token* Lexer::GetStringToken(char startDelim, char endDelim)
	{
		m_TokenBuffer.clear();
		AssertNextChar('"');

		// 读取 startDelim 和 endDelim 之间的字符
		for (char c = NextChar(); c != endDelim && c != EOF; c = NextChar()) {
			if (c == '\\') {
				// TODO: 处理 \ 符号，例如 \n
				continue;
			}

			m_TokenBuffer += c;
		}

		return CreateToken(new Token(TokenType::String, m_TokenBuffer.c_str()));
	}

	/// <summary>
	/// 可否被视为单个运算符
	/// </summary>
	/// <param name="op">运算符</param>
	/// <returns></returns>
	static bool TreatedAsOneOperator(char op)
	{
		return op == '(' || op == '[' || op == ',' || op == '.' || op == '*' || op == '?';
	}

	/// <summary>
	/// 是单个运算符
	/// </summary>
	/// <param name="op">运算符</param>
	/// <returns></returns>
	static bool IsSingleOperator(char op)
	{
		return singleOperators.find(op) != singleOperators.end();
	}

	/// <summary>
	/// 运算符是否有效
	/// </summary>
	/// <param name="op">运算符</param>
	/// <returns></returns>
	static bool OperatorValid(const std::string& op)
	{
		return validOperators.find(op) != validOperators.end();
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="str"></param>
	/// <returns></returns>
	static bool IsKeyword(const std::string& str)
	{
		return keywords.find(str) != keywords.end();
	}

	const char* Lexer::ReadOperator()
	{
		bool singleOperator = true;	// 是否为单个运算符
		char op = NextChar();

		m_TokenBuffer.clear();
		m_TokenBuffer += op;

		// 不能被视为单个运算符
		if (!TreatedAsOneOperator(op)) {
			op = PeekChar();	// 查看下一个字符
			if (IsSingleOperator(op)) {
				m_TokenBuffer += op;
				NextChar();
				singleOperator = false;	// 不是单个运算符
			}
		}

		// 不是单个运算符
		if (!singleOperator) {
			// 是无效运算符
			if (!OperatorValid(m_TokenBuffer)) {
				PushChar();	// 回退一个字符到输入流
				if (m_TokenBuffer.length() > 0) {
					m_TokenBuffer.pop_back();
				}
			}
		}
		else if (!OperatorValid(m_TokenBuffer)) {
			CompilerError(m_Compiler, "The operator %s is not valid.", m_TokenBuffer.c_str());	// 无效的运算符
		}

		return m_TokenBuffer.c_str();
	}

	void Lexer::NewExpression()
	{
		m_CurrentExpressionCount++;
		if (m_CurrentExpressionCount == 1) {
			m_ParenthesesBuffer = "";
		}
	}

	void Lexer::FinishExpression()
	{
		m_CurrentExpressionCount--;
		if (m_CurrentExpressionCount < 0) {
			CompilerError(m_Compiler, "You closed an expression that you never opened.");
		}
	}

	bool Lexer::IsInExpression()
	{
		return m_CurrentExpressionCount > 0;
	}

	Token* Lexer::GetOperatorOrStringToken()
	{
		char op = PeekChar();
		// #include <...>
		if (op == '<') {
			Token* last_token = LastToken();
			// 是 include 关键字
			if (last_token->IsKeyword("include")) {
				return GetStringToken('<', '>');
			}
		}

		Token* operatorToken = CreateToken(new Token(TokenType::Operator, ReadOperator()));

		if (op == '(') {
			// TODO 计算表达式
			NewExpression();
		}

		return operatorToken;
	}

	Token* Lexer::GetOneLineCommentToken()
	{
		m_TokenBuffer.clear();

		// 读取一行字符 直到换行或文件结束
		for (char c = PeekChar(); c != '\n' && c != EOF; c = PeekChar()) {
			m_TokenBuffer += c;
			NextChar();
		}
		return CreateToken(new Token(TokenType::Comment, m_TokenBuffer.c_str()));;
	}

	Token* Lexer::GetMultiLineCommentToken()
	{
		m_TokenBuffer.clear();
		char c = 0;

		while (true) {
			// 读取一行字符 直到*或文件结束
			for (c = PeekChar(); c != '*' && c != EOF; c = PeekChar()) {
				m_TokenBuffer += c;
				NextChar();
			}
			// 注释没有结束 文件结束
			if (c == EOF) {
				CompilerError(m_Compiler, "You did not close this multi comment.");
			}
			else if (c == '*') {
				NextChar();
				// 注释结束
				if (PeekChar() == '/') {
					NextChar();
					break;
				}
			}
		}

		return CreateToken(new Token(TokenType::Comment, m_TokenBuffer.c_str()));;;
	}

	Token* Lexer::HandleComment()
	{
		char c = PeekChar();
		// 开始注释
		if (c == '/') {
			NextChar();
			// 单行注释
			if (PeekChar() == '/') {
				NextChar();
				return GetOneLineCommentToken();
			}
			// 多行注释
			else if (PeekChar() == '*') {
				NextChar();
				return GetMultiLineCommentToken();
			}
			// 除号 返回输入流
			PushChar();
			return GetOperatorOrStringToken();
		}
		return nullptr;
	}

	Token* Lexer::GetSymbolToken()
	{
		char c = NextChar();

		if (c == ')') {
			FinishExpression();
		}

		return CreateToken(new Token(TokenType::Symbol, c));
	}

	Token* Lexer::GetIdentifierOrKeyword()
	{
		m_TokenBuffer.clear();

		for (char c = PeekChar(); (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; c = PeekChar()) {
			m_TokenBuffer += c;
			NextChar();
		}

		if (IsKeyword(m_TokenBuffer)) {
			return CreateToken(new Token(TokenType::Keyword, m_TokenBuffer.c_str()));
		}

		return CreateToken(new Token(TokenType::Identifier, m_TokenBuffer.c_str()));
	}

	Token* Lexer::ReadSpecialToken()
	{
		char c = PeekChar();

		// 标识符 或者 关键字
		if (isalpha(c) || c == '_') {
			return GetIdentifierOrKeyword();
		}
		
		return nullptr;
	}

	Token* Lexer::GetNewLineToken()
	{
		NextChar();
		return CreateToken(new Token(TokenType::NewLine));
	}

	/// <summary>
	/// 返回转义字符
	/// </summary>
	/// <param name="c">字符</param>
	/// <returns>转义字符</returns>
	static char GetEscapedChar(char c)
	{
		char co = 0;

		switch (c)
		{
			case 'n':
				co = '\n';
				break;
			case '\\':
				co = '\\';
				break;
			case 't':
				co = '\t';
				break;
			case '\'':
				co = '\'';
				break;
		}

		return co;
	}

	Token* Lexer::GetQuoteToken()
	{
		AssertNextChar('\'');
		char c = NextChar();	// 字符内容
		// \n
		if (c == '\\') {
			c = NextChar();			// c = 'n'
			c = GetEscapedChar(c);	// 返回转义字符
		}

		// 下一个字符不是 ' 单引号
		if (NextChar() != '\'') {
			CompilerError(m_Compiler, "You opened a quote ' but did not close it with a ' character.");
		}

		return CreateToken(new Token(TokenType::Number, c));;
	}

	Token* Lexer::GetNextToken()
	{
		Token* token = nullptr;

		char c = PeekChar();	// 查看下一个字符

		token = HandleComment();	// 处理注释
		if (token) {
			return token;
		}

		switch (c) {
			/* Number */
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				token = GetNumberToken();			// 返回 Number Token
				break;
			/* String */
			case '"':
				token = GetStringToken('"', '"');	// 返回 String Token
				break;
			/* Char */
			case '\'':
				token = GetQuoteToken();
				break;
			/* Operator */
			case '+':
			case '-':
			case '*':
			case '>':
			case '<':
			case '^':
			case '%':
			case '!':
			case '=':
			case '~':
			case '|':
			case '&':
			case '(':
			case '[':
			case ',':
			case '.':
			case '?':
				token = GetOperatorOrStringToken();
				break;
			/* Symbol */
			case '{':
			case '}':
			case ':':
			case ';':
			case '#':
			case '\\':
			case ')':
			case ']':
				token = GetSymbolToken();
				break;
			/* 处理空白字符返回下一个Token */
			case ' ':
			case '\t':
				token = HandleWhiteSpace();
				break;
			/* NewLine */
			case '\n':
				token = GetNewLineToken();
				break;
			/* 文件结尾 */
			case EOF:
				// TODO: 已完成词法分析
				break;
			default:
				token = ReadSpecialToken();
				if (!token) {
					CompilerError(m_Compiler, "Unexpected token.");	// 无效的 Token
				}
				break;
		}

		return token;
	}
}