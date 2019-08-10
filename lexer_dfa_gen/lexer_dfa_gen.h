#include <vector>
#include <string>
#include "../dfa/dfa.h"

struct LexerGen
{
    Dfa unionDfa;
    LexerGen();
    static Dfa CreateEmptyDfaForTokenType(int statesCnt, std::vector<int> finalStates, TokenType tokenType);
    Dfa CreateKeywordDfa(std::string kw, TokenType tokenType);
};