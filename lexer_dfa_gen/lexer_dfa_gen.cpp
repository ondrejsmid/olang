#include <iostream>
#include "lexer_dfa_gen.h"

using namespace std;

LexerGen::LexerGen()
{
    map<int, DfaUnionResolvePriority> finalStates1;
    finalStates1[1] = DfaUnionResolvePriority::Other;
    Dfa ifKwDfa(3, finalStates1);
    ifKwDfa.AddTransition(0, 1, 'i');
    ifKwDfa.AddTransition(0, 1, 'i');
    cout << ifKwDfa;
    
    cout << "----------------------------";

    map<int, DfaUnionResolvePriority> finalStates2;
    finalStates2[1] = DfaUnionResolvePriority::Other;
    finalStates2[2] = DfaUnionResolvePriority::Other;
    Dfa dfa(3, finalStates2);
    dfa.AddTransition(0, 1, 'a');
    dfa.AddTransition(0, 1, '1', '9');
    dfa.AddTransition(0, 2, 'b');
    dfa.AddTransition(0, 2, 'k', 'l');
    cout << dfa;

}
