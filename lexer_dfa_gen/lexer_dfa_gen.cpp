#include <iostream>
#include "lexer_dfa_gen.h"

using namespace std;

LexerGen::LexerGen()
{
    map<int, DfaUnionResolvePriority> finalStates1;
    finalStates1[2] = DfaUnionResolvePriority::Keyword;
    Dfa ifKwDfa(3, finalStates1);
    ifKwDfa.AddTransition(0, 1, 'i');
    ifKwDfa.AddTransition(1, 2, 'f');
    cout << ifKwDfa;
    
    cout << "----------------------------" << endl;

    map<int, DfaUnionResolvePriority> finalStates2;
    finalStates2[1] = DfaUnionResolvePriority::Other;
    finalStates2[2] = DfaUnionResolvePriority::Other;
    Dfa dfa2(3, finalStates2);
    dfa2.AddTransition(0, 1, 'a');
    dfa2.AddTransition(0, 1, '1', '9');
    dfa2.AddTransition(0, 2, 'b');
    dfa2.AddTransition(0, 2, 'k', 'l');
    cout << dfa2;

    cout << "----------------------------" << endl;

    Dfa unionDfa = ifKwDfa.Union(dfa2);
    cout << unionDfa;
}
