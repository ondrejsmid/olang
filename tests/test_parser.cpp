#include <cassert>
#include "../parser/parser.h"

void Parse_ParsesProgram()
{
    char *text =
        " a = 5; "
        "b = 0 ; "
        " c =  -2; ";
    Parser parser(text, strlen(text));
    auto ast = parser.Parse();

    assert(ast->statements.size() == 3);

    delete ast;
}

int main()
{
    Parse_ParsesProgram();
}