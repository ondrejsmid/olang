#include <iostream>
#include "lexer/lexer.h"

using namespace std;

int main()
{
    char *text = "abc";
    Lexer lexer(text, sizeof(*text));
}
