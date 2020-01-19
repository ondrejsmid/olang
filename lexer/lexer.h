#include <cstring>
#include "../lexer_gen/lexer_gen.h"
#include "../dfa/dfa.h"

#ifndef LEXER_H
#define LEXER_H

struct Token
{
    TokenType tokenType;
    int startIdx;
    int endIdx;
    Token(){};
    Token(TokenType tokenType, int startIdx, int endIdx)
        :
        tokenType(tokenType),
        startIdx(startIdx),
        endIdx(endIdx
        ) {};
    bool operator==(const Token& right)
    {
        return tokenType == right.tokenType && startIdx == right.startIdx && endIdx == right.endIdx;
    }
};

class Lexer
{
    Dfa unionDfa;
    char * text;
    size_t textLen;
    size_t idx;
    size_t prevIdx;

public:
    Lexer(char *text, size_t textLen);
    bool HasNextToken();
	Token GetNextToken();
	Token GetNextNonWhitespaceToken();
    void MoveBack();
    size_t CurrentStartIdx();
    std::string GetTokenText(const Token& token);
};

#endif