#include "CompileProcess.h"

namespace Compiler
{
	CompileProcess::CompileProcess(const char* fileName, const char* outFileName, int flags)
		: m_Flags(flags)
	{
		m_CFile.File.open(fileName, std::ios::in);		// �������ļ�
		if (!m_CFile.File.is_open()) {
			// TODO: �����ļ�������
		}

		if (outFileName) {
			m_OutFile.open(outFileName, std::ios::out);	// ������ļ�
			if (!m_OutFile.is_open()) {
				// TODO: ����ļ�������
			}
		}


	}
}