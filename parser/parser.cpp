#include "parser.h"
#include <stdexcept>
#include "parser.h"

using namespace std;

ProgramNode::~ProgramNode()
{
    for (auto i = 0; i < statements.size(); i++)
    {
        delete statements[i];
    }
}

AssignmentNode::AssignmentNode()
    :
    rightSideExpr(NULL)
{
}

AssignmentNode::~AssignmentNode()
{
    delete rightSideExpr;
}

Parser::Parser(char *text, size_t textLen)
    :
    lexer(text, textLen)
{
}

Token Parser::GetTokenThrowExceptionIfWrongType(TokenType tokenType)
{
    auto token = lexer.GetNextNonWhitespaceToken();
    if (token.tokenType != tokenType)
    {
        throw runtime_error("parse error");
    }
    return token;
}

ProgramNode * Parser::Parse()
{
    auto programNode = new ProgramNode();
    while (true)
    {
        auto token = lexer.GetNextNonWhitespaceToken();
        switch (token.tokenType)
        {
            case TokenType::VariableName:
                programNode->statements.push_back(ParseAssignment(token));
                
                break;
            case TokenType::Eof:
                return programNode;
            default:
                throw runtime_error("parse error");
        }
    }
}

AssignmentNode * Parser::ParseAssignment(const Token & variableNameToken)
{
    auto assignmentNode = new AssignmentNode();
    assignmentNode->variableNameToken = variableNameToken;
    assignmentNode->assignmentToken = GetTokenThrowExceptionIfWrongType(TokenType::Assignment);
    assignmentNode->rightSideExpr = ParseExpr();
    assignmentNode->semicolonToken = GetTokenThrowExceptionIfWrongType(TokenType::Semicolon);
    return assignmentNode;
}

ExprNode * Parser::ParseExpr()
{
    auto token = lexer.GetNextNonWhitespaceToken();
    switch (token.tokenType)
    {
    case TokenType::Number:
    {
        auto numberNode = new NumberNode();
        numberNode->numberToken = token;
        return numberNode;
    }

    case TokenType::VariableName:
    {
        auto rightSideVariableNode = new RightSideVariableNode();
        rightSideVariableNode->variableNameToken = token;
        return rightSideVariableNode;
    }

    default:
        throw runtime_error("parse error");
    }
}

