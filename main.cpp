#include <iostream>
#include <list>
#include <string.h>
#include "parser/parser.h"

using namespace std;

int main()
{
    char *text =
        " a = 5; "
        "b = 0 ; "
        " c =  -2; ";
    Parser parser(text, strlen(text));
    auto ast = parser.Parse();
    delete ast;
}
