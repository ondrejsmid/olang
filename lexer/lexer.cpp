#include "lexer.h"

Lexer::Lexer(char * text, size_t textLen)
{
    LexerGen lexerGen;
    unionDfa = lexerGen.unionDfa;
}
