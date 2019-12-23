#include <vector>
#include <string>
#include "../dfa/dfa.h"

#ifndef LEXER_GEN_H
#define LEXER_GEN_H

struct LexerGen
{
    Dfa unionDfa;
    LexerGen();
    static Dfa CreateEmptyDfaForTokenType(int statesCnt, std::vector<int> finalStates, TokenType tokenType);
    Dfa CreateKeywordDfa(std::string kw, TokenType tokenType);
};

#endif