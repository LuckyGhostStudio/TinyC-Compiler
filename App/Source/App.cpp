#include <iostream>

#include "Core/Compiler.h"

using namespace Compiler;

int main()
{
	CompileState result = CompileFile("../TestFiles/test.c", "../TestFiles/test", 0);
	if (result == CompileState::FileCompiledOK) {
		std::cout << "Everything Compiled Fine\n";
	}
	else if (result == CompileState::FailedWithErrors) {
		std::cout << "Compile Faild\n";
	}
	else {
		std::cout << "Unknow Errors\n";
	}

	return 0;
}