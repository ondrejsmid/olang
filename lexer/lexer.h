#include "../lexer_gen/lexer_gen.h"
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
    char * text;
    size_t textLen;
    size_t idx;

public:
    Lexer(char *text, size_t textLen);
    bool HasNextToken();
    Token GetNextToken();
};