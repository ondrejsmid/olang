#include <vector>
#include "dfa.h"

struct Token
{
};

enum KeywordEnum
{
    IF,
    ELSE,
    WHILE
};

struct Keyword : Token
{
    KeywordEnum kwEnum;
};

class LexerGen
{
private:
    std::vector<Dfa> tokenDfas;
public:
    LexerGen();
};