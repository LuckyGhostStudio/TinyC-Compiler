#include "Parser.h"

namespace Compiler
{
    int Parser::Next()
    {
        return 1;
    }

    Parser::Parser(CompileProcess* compiler)
    {
        m_Compiler = compiler;
    }

    ParseState Compiler::Parser::Parse()
    {
        Node* node = nullptr;

        // TODO：遍历所有token 创建node
        //m_Compiler->m_Tokens.back
        while (Next() == 0) {
            //node = NodePeek();
            m_Compiler->m_NodeTrees.push_back(node);
        }

        return ParseState::Successfully;
    }
}