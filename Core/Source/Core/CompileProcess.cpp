#include "CompileProcess.h"

namespace Compiler
{
	CompileProcess::CompileProcess(const char* fileName, const char* outFileName, int flags)
		: m_Flags(flags)
	{
		m_CFile.File.open(fileName, std::ios::in);		// 输入流文件
		if (!m_CFile.File.is_open()) {
			// TODO: 输入文件不存在
		}

		if (outFileName) {
			m_OutFile.open(outFileName, std::ios::out);	// 输出流文件
			if (!m_OutFile.is_open()) {
				// TODO: 输出文件不存在
			}
		}


	}
}