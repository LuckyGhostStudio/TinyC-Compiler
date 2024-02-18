#include <iostream>

#include "Core/Compiler.h"

using namespace Compiler;

int main(const char* argv)
{
	CompileState state = CompileFile("TestFiles\\test.c", "TestFiles\\test", 0);	// ±àÒëÎÄ¼ş

	if (state == CompileState::Successfully) {
		std::cout << "Everything Compiled Fine\n";
	}
	else if (state == CompileState::FailedWithErrors) {
		std::cout << "Compile Faild\n";
	}
	else {
		std::cout << "Unknow Errors\n";
	}

	return 0;
}