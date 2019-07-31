#include <iostream>
#include <list>
#include "lexer_dfa_gen.h"

using namespace std;

LexerGen::LexerGen()
{
    list<Dfa*> allTokenDfas;
    
    Dfa variableNameDfa = CreateEmptyDfaForTokenType(2, vector<int>{1}, TokenType::VariableName);
    variableNameDfa.AddTransition(0, 1, 'A', 'Z');
    variableNameDfa.AddTransition(0, 1, 'a', 'z');
    variableNameDfa.AddTransition(1, 1, 'A', 'Z');
    variableNameDfa.AddTransition(1, 1, 'a', 'z');
    variableNameDfa.AddTransition(1, 1, '0', '9');
    allTokenDfas.push_back(&variableNameDfa);

    Dfa numberDfa = CreateEmptyDfaForTokenType(4, vector<int>{1, 2}, TokenType::Number);
    numberDfa.AddTransition(0, 1, '0');
    numberDfa.AddTransition(0, 2, '1', '9');
    numberDfa.AddTransition(2, 2, '1', '9');
    numberDfa.AddTransition(0, 3, '-');
    numberDfa.AddTransition(3, 2, '1', '9');
    allTokenDfas.push_back(&numberDfa);

    Dfa ifDfa = CreateEmptyDfaForTokenType(3, vector<int>{2}, TokenType::If);
    ifDfa.AddTransition(0, 1, 'i');
    ifDfa.AddTransition(1, 2, 'f');
    allTokenDfas.push_back(&ifDfa);
    
    list<Dfa*>::const_iterator tokenDfa = allTokenDfas.begin();
    Dfa unionDfa = **tokenDfa;
    ++tokenDfa;

    while (tokenDfa != allTokenDfas.end())
    {
        unionDfa = unionDfa.Union(**tokenDfa);
        ++tokenDfa;
    }
    this->unionDfa = unionDfa;
}

Dfa LexerGen::CreateEmptyDfaForTokenType(int statesCnt, std::vector<int> finalStates, TokenType tokenType)
{
    map<int, TokenType> finalStatesWithTokType;
    for (int i = 0; i < finalStates.size(); i++)
    {
        finalStatesWithTokType[finalStates[i]] = tokenType;
    }
    Dfa emptyDfa(statesCnt, finalStatesWithTokType);
    return emptyDfa;
}
