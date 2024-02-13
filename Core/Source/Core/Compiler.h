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
	/// Token����
	/// </summary>
	enum class TokenType
	{
		Identifier,	// ��ʶ��
		Keyword,	// �ؼ���	int float ...
		Operator,	// �����	+ - * / ...
		Symbol,		// ����		; : " ...
		Number,		// ����		0 1 2 3 ...
		String,		// �ַ���	"Hello"
		Comment,	// ע��		// ... /* ... */
		NewLine		// ����
	};

	struct Token
	{
		TokenType type;
		int flags;

		/* token ֵ */
		union
		{
			char charValue;						// char ����ֵ
			const char* stringValue;			// string ����ֵ
			unsigned int intNumber;				// int ����ֵ
			unsigned long longNumber;			// long ����ֵ
			unsigned long long longlongNumber;	// longlong ����ֵ
			void* any;							// ��������ֵ
		};

		bool whiteSpace;				// ��ǰtoken����һ��token֮���Ƿ���ڿհף��ո���Ŀհ׷���

		const char* between_brackets;	// ����֮����ַ���ֵ�������ã�
	};

	enum class CompileState
	{
		FileCompiledOK,		// �ļ�����ɹ�
		FailedWithErrors,	// ����
	};

	/// <summary>
	/// �������
	/// </summary>
	struct CompileProcess
	{
		int flags;	// �ļ����뷽ʽ�ı�־

		/// <summary>
		/// �����ļ��������
		/// </summary>
		struct InputFileCompileProcess
		{
			FILE* file;					// �ļ�
			const char* absolutePath;	// ����·��
		} cFile;

		FILE* outFile;	// ����ļ�
	};

	/// <summary>
	/// �����ļ�
	/// </summary>
	/// <param name="fileName">�����ļ���</param>
	/// <param name="outFileName">����ļ���</param>
	/// <param name="flags">�ļ����뷽ʽ�ı�־</param>
	/// <returns>������״̬</returns>
	CompileState CompileFile(const char* fileName, const char* outFileName, int flags);

	/// <summary>
	/// �����������
	/// </summary>
	/// <param name="fileName">�����ļ���</param>
	/// <param name="outFileName">����ļ���</param>
	/// <param name="flags">�ļ����뷽ʽ�ı�־</param>
	/// <returns>�������</returns>
	CompileProcess* CreateCompileProcess(const char* fileName, const char* outFileName, int flags);
}