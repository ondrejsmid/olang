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
    assignmentNode->rightSideExpr = ParseExpr(&(assignmentNode->semicolonToken));
    return assignmentNode;
}

ExprNode * Parser::ParseExpr(Token * semicolonToken)
{
    AssocOperationNode* assocOperation;
    ExprNode* currentExprInAssocList;

    bool operatorFound = false;

    while (true)
    {
        auto token = lexer.GetNextNonWhitespaceToken();
        switch (token.tokenType)
        {
        case TokenType::Number:
        {
            auto numberNode = new NumberNode();
            numberNode->numberToken = token;
            currentExprInAssocList = numberNode;
            break;
        }

        case TokenType::VariableName:
        {
            auto rightSideVariableNode = new RightSideVariableNode();
            rightSideVariableNode->variableNameToken = token;
            currentExprInAssocList = rightSideVariableNode;
            break;
        }

        case TokenType::Plus:
            if (!operatorFound)
            {
                assocOperation = new AssocOperationNode();
                operatorFound = true;
            }
            assocOperation->operands.push_back(currentExprInAssocList);
            assocOperation->tokensBetweenOperands.push_back(token);
            break;

        case TokenType::Semicolon:
        {
            *semicolonToken = token;
            if (operatorFound)
            {
                assocOperation->operands.push_back(currentExprInAssocList);
                return assocOperation;
            }
            else {
                return currentExprInAssocList;
            }
        }

        default:
            throw runtime_error("parse error");
        }
    }
    throw runtime_error("parse error");
}

