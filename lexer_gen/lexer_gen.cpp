#include <iostream>
#include <list>
#include "lexer_gen.h"

using namespace std;

LexerGen::LexerGen()
{
    list<Dfa> allTokenDfas;
    
    Dfa whitespaceDfa = CreateEmptyDfaForTokenType(2, vector<int>{1}, TokenType::Whitespace);
    whitespaceDfa.AddTransition(0, 1, ' ');
    whitespaceDfa.AddTransition(0, 1, '\t');
    whitespaceDfa.AddTransition(0, 1, '\n');
    whitespaceDfa.AddTransition(0, 1, '\r');
    whitespaceDfa.AddTransition(1, 1, ' ');
    whitespaceDfa.AddTransition(1, 1, '\t');
    whitespaceDfa.AddTransition(1, 1, '\n');
    whitespaceDfa.AddTransition(1, 1, '\r');
    allTokenDfas.push_back(whitespaceDfa);

    Dfa variableNameDfa = CreateEmptyDfaForTokenType(2, vector<int>{1}, TokenType::VariableName);
    variableNameDfa.AddTransition(0, 1, 'A', 'Z');
    variableNameDfa.AddTransition(0, 1, 'a', 'z');
    variableNameDfa.AddTransition(1, 1, 'A', 'Z');
    variableNameDfa.AddTransition(1, 1, 'a', 'z');
    variableNameDfa.AddTransition(1, 1, '0', '9');
    allTokenDfas.push_back(variableNameDfa);

    Dfa numberDfa = CreateEmptyDfaForTokenType(4, vector<int>{1, 2}, TokenType::Number);
    numberDfa.AddTransition(0, 1, '0');
    numberDfa.AddTransition(0, 2, '1', '9');
    numberDfa.AddTransition(2, 2, '1', '9');
    numberDfa.AddTransition(0, 3, '-');
    numberDfa.AddTransition(3, 2, '1', '9');
    allTokenDfas.push_back(numberDfa);

    allTokenDfas.push_back(CreateKeywordDfa(";", TokenType::Semicolon));

    allTokenDfas.push_back(CreateKeywordDfa("=", TokenType::Assign));

    allTokenDfas.push_back(CreateKeywordDfa("+", TokenType::Plus));

    allTokenDfas.push_back(CreateKeywordDfa("if", TokenType::If));

    allTokenDfas.push_back(CreateKeywordDfa("(", TokenType::RoundLeftBracket));

    allTokenDfas.push_back(CreateKeywordDfa(")", TokenType::RoundRightBracket));

    list<Dfa>::iterator tokenDfa = allTokenDfas.begin();
    Dfa unionDfa = *tokenDfa;
    ++tokenDfa;

    while (tokenDfa != allTokenDfas.end())
    {
        unionDfa = unionDfa.Union(*tokenDfa);
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

#pragma warning( disable : 4267 ) /* Keyword length will never be bigger that max int. */
Dfa LexerGen::CreateKeywordDfa(std::string kw, TokenType tokenType)
{
    Dfa kwDfa = CreateEmptyDfaForTokenType(kw.size() + 1, vector<int>{(int)kw.size()}, tokenType);
    for (int i = 0; i < kw.size(); i++)
    {
        kwDfa.AddTransition(i, i + 1, kw[i]);
    }
    return kwDfa;
}
