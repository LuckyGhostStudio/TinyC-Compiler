#include "Lexer.h"
#include "Compiler.h"

#include <cstring>
#include <ctype.h>

namespace Compiler
{
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
		NextChar();

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
		return op == '+' ||
			   op == '-' ||
			   op == '*' ||	// -
			   op == '/' ||
			   op == '=' ||
			   op == '>' ||
			   op == '<' ||
			   op == '|' ||
			   op == '&' ||
			   op == '!' ||
			   op == '^' ||
			   op == '~' ||
			   op == '%' ||
			   op == '(' ||	// -
			   op == '[' ||	// -
			   op == ',' ||	// -
			   op == '.' ||	// -
			   op == '?';	// -
	}

	/// <summary>
	/// 运算符是否有效
	/// </summary>
	/// <param name="op">运算符</param>
	/// <returns></returns>
	static bool OperatorValid(const char* op)
	{
		std::string opStr = op;
		return opStr == "+" ||
			   opStr == "-" ||
			   opStr == "*" ||
			   opStr == "/" ||
			   opStr == "&" ||
			   opStr == "|" ||
			   opStr == "!" ||
			   opStr == "^" ||
			   opStr == ">" ||
			   opStr == "<" ||
			   opStr == "++" ||
			   opStr == "--" ||
			   opStr == "=" ||
			   opStr == "+=" ||
			   opStr == "-=" ||
			   opStr == "*=" ||
			   opStr == "/=" ||
			   opStr == ">>" ||
			   opStr == "<<" ||
			   opStr == ">=" ||
			   opStr == "<=" ||
			   opStr == "&&" ||
			   opStr == "||" ||
			   opStr == "!=" ||
			   opStr == "==" ||
			   opStr == "(" ||
			   opStr == "[" ||
			   opStr == "," ||
			   opStr == "." ||
			   opStr == "->" ||
			   opStr == "..." ||	// TODO delete
			   opStr == "~" ||
			   opStr == "?" ||
			   opStr == "%";
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
			if (!OperatorValid(m_TokenBuffer.c_str())) {
				PushChar();	// 回退一个字符到输入流
				if (m_TokenBuffer.length() > 0) {
					m_TokenBuffer.pop_back();
				}
			}
		}
		else if (!OperatorValid(m_TokenBuffer.c_str())) {
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

	Token* Lexer::GetNextToken()
	{
		Token* token = nullptr;

		char c = PeekChar();	// 查看下一个字符
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
			case ' ':
			case '\t':
			case '\n':
				token = HandleWhiteSpace();			// 处理空白字符返回下一个Token
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