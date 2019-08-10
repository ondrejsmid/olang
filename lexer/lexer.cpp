#include "lexer.h"

Lexer::Lexer(char * text, size_t textLen)
    :
    text(text),
    textLen(textLen),
    idx(0)
{
    LexerGen lexerGen;
    unionDfa = lexerGen.unionDfa;
}

bool Lexer::HasNextToken()
{
    return idx != textLen;
}

Token Lexer::GetNextToken()
{
    unionDfa.Reset();
    size_t tokenStartIdx = idx;
    while (idx != textLen && !unionDfa.GoesToErrorState(text[idx]))
    {
        unionDfa.Move(text[idx]);
        idx++;
    }
    if (unionDfa.IsInFinalState())
    {
        Token token;
        token.tokenType = unionDfa.CurrentFinalStateTokenType();
        token.startIdx = tokenStartIdx;
        token.endIdx = idx - 1;
        return token;
    }
    else {
        Token token;
        token.tokenType = TokenType::Invalid;
        return token;
    }
}
