#include "Lexer.h"
#include "Compiler.h"

#include <cstring>
#include <ctype.h>
#include <unordered_set>
#include <cassert>

namespace Compiler
{
	/* ��������� */
	static std::unordered_set<char> singleOperators = {
		'+', '-', '/',	'=', '%', '>', '<', '|', '&', '!', '^', '~',
		'*', '(', '[', ',', '.', '?'
	};

	/* ��Ч������� */
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

	/* ֧�ֵĹؼ��� */
	static std::unordered_set<std::string> keywords = {
		"unsigned", "signed", "char", "short", "int", "long", "float", "double", "void",
		"struct", "union", "static", "typedef", "const",
		"include", "sizeof",
		"if", "else", "while", "for", "do", "break", "continue", "switch", "case", "default", "goto", "return",
		"extern", "restrict",
		"__ignore_typecheck"	// �������ͼ��
	};

	Lexer::Lexer(CompileProcess* compiler, void* privateData)
	{
		m_Compiler = compiler;
		m_PrivateData = privateData;
	}

	char Lexer::NextChar()
	{
		m_Compiler->m_CharPos.Column++;			// ����+1
		m_CharPos.Column++;
		char c = static_cast<char>(m_Compiler->m_CFile.File.get());	// ���ļ���������ȡһ���ַ�����ȡ����ַ����ļ���ɾ����
		// ��ǰ�ַ�Ϊ���з�
		if (c == '\n') {
			m_Compiler->m_CharPos.Line++;			// ����+1
			m_CharPos.Line++;
			m_Compiler->m_CharPos.Column = 1;		// ��������
			m_CharPos.Column = 1;
		}

		return c;
	}

	char Lexer::PeekChar()
	{
		return static_cast<char>(m_Compiler->m_CFile.File.peek());	// ���ļ���������ȡһ���ַ�
	}

	void Lexer::PushChar(/*char c*/)
	{
		// ungetc(c, m_Compiler->m_CFile.m_File);		// TODO: �����ַ����ļ�������
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

		/* ��ȡ���� Token */
		Token* token = GetNextToken();
		while (token) {
			m_Tokens.push_back(new Token(*token));	// ��ӵ�Tokens�б�

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
		Token* lastToken = LastToken();		// ���һ��token
		if (lastToken) {
			lastToken->m_WhiteSpace = true;
		}

		NextChar();
		return GetNextToken();
	}

	const char* Lexer::ReadNumberStr()
	{
		m_TokenBuffer.clear();

		// ��ȡ������ 0-9 ���ַ���Buffer
		for (char c = PeekChar(); c >= '0' && c <= '9'; c = PeekChar()) {
			m_TokenBuffer += c;
			NextChar();
		}

		return m_TokenBuffer.c_str();
	}

	unsigned long long Lexer::ReadNumber()
	{
		return strtoull(ReadNumberStr(), nullptr, 10);	// תΪ long long ʮ��������
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

		// ��ȡ startDelim �� endDelim ֮����ַ�
		for (char c = NextChar(); c != endDelim && c != EOF; c = NextChar()) {
			if (c == '\\') {
				// TODO: ���� \ ���ţ����� \n
				continue;
			}

			m_TokenBuffer += c;
		}

		return CreateToken(new Token(TokenType::String, m_TokenBuffer.c_str()));
	}

	/// <summary>
	/// �ɷ���Ϊ���������
	/// </summary>
	/// <param name="op">�����</param>
	/// <returns></returns>
	static bool TreatedAsOneOperator(char op)
	{
		return op == '(' || op == '[' || op == ',' || op == '.' || op == '*' || op == '?';
	}

	/// <summary>
	/// �ǵ��������
	/// </summary>
	/// <param name="op">�����</param>
	/// <returns></returns>
	static bool IsSingleOperator(char op)
	{
		return singleOperators.find(op) != singleOperators.end();
	}

	/// <summary>
	/// ������Ƿ���Ч
	/// </summary>
	/// <param name="op">�����</param>
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
		bool singleOperator = true;	// �Ƿ�Ϊ���������
		char op = NextChar();

		m_TokenBuffer.clear();
		m_TokenBuffer += op;

		// ���ܱ���Ϊ���������
		if (!TreatedAsOneOperator(op)) {
			op = PeekChar();	// �鿴��һ���ַ�
			if (IsSingleOperator(op)) {
				m_TokenBuffer += op;
				NextChar();
				singleOperator = false;	// ���ǵ��������
			}
		}

		// ���ǵ��������
		if (!singleOperator) {
			// ����Ч�����
			if (!OperatorValid(m_TokenBuffer)) {
				PushChar();	// ����һ���ַ���������
				if (m_TokenBuffer.length() > 0) {
					m_TokenBuffer.pop_back();
				}
			}
		}
		else if (!OperatorValid(m_TokenBuffer)) {
			CompilerError(m_Compiler, "The operator %s is not valid.", m_TokenBuffer.c_str());	// ��Ч�������
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
			// �� include �ؼ���
			if (last_token->IsKeyword("include")) {
				return GetStringToken('<', '>');
			}
		}

		Token* operatorToken = CreateToken(new Token(TokenType::Operator, ReadOperator()));

		if (op == '(') {
			// TODO ������ʽ
			NewExpression();
		}

		return operatorToken;
	}

	Token* Lexer::GetOneLineCommentToken()
	{
		m_TokenBuffer.clear();

		// ��ȡһ���ַ� ֱ�����л��ļ�����
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
			// ��ȡһ���ַ� ֱ��*���ļ�����
			for (c = PeekChar(); c != '*' && c != EOF; c = PeekChar()) {
				m_TokenBuffer += c;
				NextChar();
			}
			// ע��û�н��� �ļ�����
			if (c == EOF) {
				CompilerError(m_Compiler, "You did not close this multi comment.");
			}
			else if (c == '*') {
				NextChar();
				// ע�ͽ���
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
		// ��ʼע��
		if (c == '/') {
			NextChar();
			// ����ע��
			if (PeekChar() == '/') {
				NextChar();
				return GetOneLineCommentToken();
			}
			// ����ע��
			else if (PeekChar() == '*') {
				NextChar();
				return GetMultiLineCommentToken();
			}
			// ���� ����������
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

		// ��ʶ�� ���� �ؼ���
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
	/// ����ת���ַ�
	/// </summary>
	/// <param name="c">�ַ�</param>
	/// <returns>ת���ַ�</returns>
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
		char c = NextChar();	// �ַ�����
		// \n
		if (c == '\\') {
			c = NextChar();			// c = 'n'
			c = GetEscapedChar(c);	// ����ת���ַ�
		}

		// ��һ���ַ����� ' ������
		if (NextChar() != '\'') {
			CompilerError(m_Compiler, "You opened a quote ' but did not close it with a ' character.");
		}

		return CreateToken(new Token(TokenType::Number, c));;
	}

	Token* Lexer::GetNextToken()
	{
		Token* token = nullptr;

		char c = PeekChar();	// �鿴��һ���ַ�

		token = HandleComment();	// ����ע��
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
				token = GetNumberToken();			// ���� Number Token
				break;
			/* String */
			case '"':
				token = GetStringToken('"', '"');	// ���� String Token
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
			/* ����հ��ַ�������һ��Token */
			case ' ':
			case '\t':
				token = HandleWhiteSpace();
				break;
			/* NewLine */
			case '\n':
				token = GetNewLineToken();
				break;
			/* �ļ���β */
			case EOF:
				// TODO: ����ɴʷ�����
				break;
			default:
				token = ReadSpecialToken();
				if (!token) {
					CompilerError(m_Compiler, "Unexpected token.");	// ��Ч�� Token
				}
				break;
		}

		return token;
	}
}