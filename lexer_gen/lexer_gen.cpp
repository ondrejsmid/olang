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

    Dfa stringDfa = CreateEmptyDfaForTokenType(4, vector<int>{3}, TokenType::String);
    stringDfa.AddTransition(0, 1, '"');
    stringDfa.AddTransition(1, 1, 0, '"' - 1);
    stringDfa.AddTransition(1, 1, '"' + 1, '\\' - 1);
    stringDfa.AddTransition(1, 1, '\\' + 1, 255);
    stringDfa.AddTransition(1, 2, '\\');
    stringDfa.AddTransition(1, 3, '"');
    stringDfa.AddTransition(2, 1, 0, 255);
    allTokenDfas.push_back(stringDfa);

    allTokenDfas.push_back(CreateKeywordDfa(";", TokenType::Semicolon));
    allTokenDfas.push_back(CreateKeywordDfa("=", TokenType::Assignment));
    allTokenDfas.push_back(CreateKeywordDfa("+", TokenType::Plus));
    allTokenDfas.push_back(CreateKeywordDfa("-", TokenType::UnaryMinus));
    allTokenDfas.push_back(CreateKeywordDfa("if", TokenType::If));
    allTokenDfas.push_back(CreateKeywordDfa("else", TokenType::Else));
    allTokenDfas.push_back(CreateKeywordDfa("(", TokenType::LeftRoundBracket));
    allTokenDfas.push_back(CreateKeywordDfa(")", TokenType::RightRoundBracket));
    allTokenDfas.push_back(CreateKeywordDfa("{", TokenType::LeftCurlyBracket));
    allTokenDfas.push_back(CreateKeywordDfa("}", TokenType::RightCurlyBracket));
    allTokenDfas.push_back(CreateKeywordDfa("true", TokenType::True));
    allTokenDfas.push_back(CreateKeywordDfa("false", TokenType::False));
    allTokenDfas.push_back(CreateKeywordDfa("==", TokenType::Equal));
    allTokenDfas.push_back(CreateKeywordDfa("!", TokenType::Negate));
    allTokenDfas.push_back(CreateKeywordDfa("!=", TokenType::NotEqual));
    allTokenDfas.push_back(CreateKeywordDfa(">", TokenType::Greater));
    allTokenDfas.push_back(CreateKeywordDfa(">=", TokenType::GreaterOrEqual));
    allTokenDfas.push_back(CreateKeywordDfa("<", TokenType::Less));
    allTokenDfas.push_back(CreateKeywordDfa("<=", TokenType::LessOrEqual));
    allTokenDfas.push_back(CreateKeywordDfa("&&", TokenType::And));
    allTokenDfas.push_back(CreateKeywordDfa("||", TokenType::Or));

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
	vector<pair<bool, TokenType>> statesFinality(statesCnt, pair<bool, TokenType>(false, TokenType::Invalid));
    for (int i = 0; i < finalStates.size(); i++)
    {
		statesFinality[finalStates[i]].first = true;
		statesFinality[finalStates[i]].second = tokenType;
    }
    Dfa emptyDfa(statesCnt, statesFinality);
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
