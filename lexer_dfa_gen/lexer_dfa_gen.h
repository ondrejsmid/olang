#include <vector>
#include "dfa.h"

struct LexerGen
{
    Dfa unionDfa;
    LexerGen();
    static Dfa CreateEmptyDfaForTokenType(int statesCnt, std::vector<int> finalStates, TokenType tokenType);
};