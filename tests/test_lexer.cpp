#include <cassert>
#include "../lexer/lexer.h"

int main()
{
    char *text = "abc  \t  \r\n xyz123 4";
    Lexer lexer(text, strlen(text));
    
    Token token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::VariableName);
    assert(token.startIdx == 0);
    assert(token.endIdx == 2);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 3);
    assert(token.endIdx == 10);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::VariableName);
    assert(token.startIdx == 11);
    assert(token.endIdx == 16);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 17);
    assert(token.endIdx == 17);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Number);
    assert(token.startIdx == 18);
    assert(token.endIdx == 18);

    assert(!lexer.HasNextToken());

    text = "if123 if";
    lexer = Lexer(text, strlen(text));

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::VariableName);
    assert(token.startIdx == 0);
    assert(token.endIdx == 4);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 5);
    assert(token.endIdx == 5);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::If);
    assert(token.startIdx == 6);
    assert(token.endIdx == 7);

    assert(!lexer.HasNextToken());

    text = "if123if";
    lexer = Lexer(text, strlen(text));

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::VariableName);
    assert(token.startIdx == 0);
    assert(token.endIdx == 6);

    assert(!lexer.HasNextToken());

    text = "(())";
    lexer = Lexer(text, strlen(text));

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::RoundLeftBracket);
    assert(token.startIdx == 0);
    assert(token.endIdx == 0);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::RoundLeftBracket);
    assert(token.startIdx == 1);
    assert(token.endIdx == 1);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::RoundRightBracket);
    assert(token.startIdx == 2);
    assert(token.endIdx == 2);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::RoundRightBracket);
    assert(token.startIdx == 3);
    assert(token.endIdx == 3);

    assert(!lexer.HasNextToken());

    text = "a = -7 + 4;x= a + \t (b+3)";
    lexer = Lexer(text, strlen(text));

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::VariableName);
    assert(token.startIdx == 0);
    assert(token.endIdx == 0);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 1);
    assert(token.endIdx == 1);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Assign);
    assert(token.startIdx == 2);
    assert(token.endIdx == 2);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 3);
    assert(token.endIdx == 3);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Number);
    assert(token.startIdx == 4);
    assert(token.endIdx == 5);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 6);
    assert(token.endIdx == 6);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Plus);
    assert(token.startIdx == 7);
    assert(token.endIdx == 7);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 8);
    assert(token.endIdx == 8);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Number);
    assert(token.startIdx == 9);
    assert(token.endIdx == 9);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Semicolon);
    assert(token.startIdx == 10);
    assert(token.endIdx == 10);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::VariableName);
    assert(token.startIdx == 11);
    assert(token.endIdx == 11);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Assign);
    assert(token.startIdx == 12);
    assert(token.endIdx == 12);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 13);
    assert(token.endIdx == 13);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::VariableName);
    assert(token.startIdx == 14);
    assert(token.endIdx == 14);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 15);
    assert(token.endIdx == 15);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Plus);
    assert(token.startIdx == 16);
    assert(token.endIdx == 16);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Whitespace);
    assert(token.startIdx == 17);
    assert(token.endIdx == 19);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::RoundLeftBracket);
    assert(token.startIdx == 20);
    assert(token.endIdx == 20);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::VariableName);
    assert(token.startIdx == 21);
    assert(token.endIdx == 21);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Plus);
    assert(token.startIdx == 22);
    assert(token.endIdx == 22);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::Number);
    assert(token.startIdx == 23);
    assert(token.endIdx == 23);

    token = lexer.GetNextToken();
    assert(token.tokenType == TokenType::RoundRightBracket);
    assert(token.startIdx == 24);
    assert(token.endIdx == 24);

    assert(!lexer.HasNextToken());
}