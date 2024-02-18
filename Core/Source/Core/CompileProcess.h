#pragma once

#include "CharPos.h"

#include <fstream>
#include <string>

namespace Compiler
{
	/// <summary>
	/// �������
	/// </summary>
	class CompileProcess
	{
	public:
		int m_Flags;		// �ļ����뷽ʽ�ı�־

		CharPos m_CharPos;	// �ַ�λ����Ϣ

		/// <summary>
		/// �����ļ�
		/// </summary>
		struct InputFile
		{
			std::ifstream File;			// �ļ�������
			std::string AbsolutePath;	// ����·��
		} m_CFile;

		std::ofstream m_OutFile;			// �ļ������

		/// <summary>
		/// �������
		/// </summary>
		/// <param name="fileName">�����ļ���</param>
		/// <param name="outFileName">����ļ���</param>
		/// <param name="flags">�ļ����뷽ʽ�ı�־</param>
		CompileProcess(const char* fileName, const char* outFileName, int flags);
	};
}