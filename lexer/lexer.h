#include "../lexer_dfa_gen/lexer_dfa_gen.h"
#include "../dfa/dfa.h"

struct Token
{
    TokenType tokenType;
    int startIdx;
    int endIdx;
};

class Lexer
{
    Dfa unionDfa;
public:
    Lexer(char *text, size_t textLen);
};