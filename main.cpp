#include <iostream>
#include <list>
#include <string.h>
#include "lexer/lexer.h"

using namespace std;

int main()
{
    char *text = "abc  \t  \r\n xyz123 4";
    Lexer lexer(text, strlen(text));
    list<Token> tokens;
    while (lexer.HasNextToken())
    {
        tokens.push_back(lexer.GetNextToken());
    }
}
