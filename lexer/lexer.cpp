#include "lexer.h"
#include "lexer.h"
#include "lexer.h"
#include <cassert>
#include "lexer.h"

using namespace std;

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
    if (idx == textLen)
    {
        Token token;
        token.tokenType = TokenType::Eof;
        return token;
    }

    prevIdx = idx;

    unionDfa.Reset();
    size_t tokenStartIdx = idx;
    while (idx != textLen && !unionDfa.GoesToErrorState(text[idx]))
    {
        unionDfa.Move(text[idx]);
        idx++;
    }
	Token token;
	token.startIdx = tokenStartIdx;
	token.endIdx = idx == tokenStartIdx ? token.startIdx : idx - 1; /* If it is a case when even the first char went to an error state,
                                                                    provide client this first char that went to the error state as a one-char Invalid token,
                                                                    instead of an Invalid token of zero size, which would not be too much helpful.
                                                                    (With this approach, token.endIdx will be equal to token.startIdx) */
    if (unionDfa.IsInFinalState())
    {
        token.tokenType = unionDfa.CurrentFinalStateTokenType();
        return token;
    }
    else {
        token.tokenType = TokenType::Invalid;
        return token;
    }
}

Token Lexer::GetNextNonWhitespaceToken()
{
	Token token = GetNextToken();
    if (token.tokenType == TokenType::Whitespace)
    {
        return GetNextToken();
    }
    else {
        return token;
    }
}

void Lexer::MoveBack()
{
    idx = prevIdx;
}

size_t Lexer::CurrentIdx()
{
    return idx;
}

std::string Lexer::GetTokenText(const Token& token)
{
    return string(text + token.startIdx, text + token.endIdx + 1);
}
