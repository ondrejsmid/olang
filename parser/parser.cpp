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
    auto program = new ProgramNode();
    while (true)
    {
        auto token = lexer.GetNextNonWhitespaceToken();
        switch (token.tokenType)
        {
            case TokenType::VariableName:
                program->statements.push_back(ParseAssignment(token));
                break;

            case TokenType::Eof:
                return program;

            default:
                throw runtime_error("parse error");
        }
    }
}

AssignmentNode * Parser::ParseAssignment(const Token & variableNameToken)
{
    auto assignment = new AssignmentNode();
    assignment->variableNameToken = variableNameToken;
    assignment->assignmentToken = GetTokenThrowExceptionIfWrongType(TokenType::Assignment);
    assignment->rightSideExpr = ParseExpr(&(assignment->semicolonToken));
    return assignment;
}

ExprNode * Parser::ParseExpr(Token * terminationToken)
{
    AssocOperationNode* assocOperation;
    ExprNode* currentExprInAssocList;

    bool plusFound = false;

    while (true)
    {
        auto token = lexer.GetNextNonWhitespaceToken();
        switch (token.tokenType)
        {
        case TokenType::Number:
        {
            auto number = new NumberNode();
            number->numberToken = token;
            currentExprInAssocList = number;
            break;
        }

        case TokenType::VariableName:
        {
            auto rightSideVariable = new RightSideVariableNode();
            rightSideVariable->variableNameToken = token;
            currentExprInAssocList = rightSideVariable;
            break;
        }

        case TokenType::String:
        {
            auto stringNode = new StringNode();
            stringNode->stringToken = token;
            currentExprInAssocList = stringNode;
            break;
        }

        case TokenType::Plus:
            if (!plusFound)
            {
                assocOperation = new AssocOperationNode();
                plusFound = true;
            }
            assocOperation->operands.push_back(currentExprInAssocList);
            assocOperation->tokensBetweenOperands.push_back(token);
            break;

        case TokenType::UnaryMinus:
        {
            auto unaryMinus = new UnaryMinusNode();
            unaryMinus->unaryMinusToken = token;
            unaryMinus->leftRoundBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftRoundBracket);
            unaryMinus->innerExpr = ParseExpr(&(unaryMinus->rightRoundBracketToken));
            currentExprInAssocList = unaryMinus;
            break;
        }

        case TokenType::LeftRoundBracket:
        {
            auto enclosedExpr = new EnclosedExpr();
            enclosedExpr->leftRoundBracketToken = token;
            enclosedExpr->innerExpr = ParseExpr(&(enclosedExpr->rightRoundBracketToken));
            currentExprInAssocList = enclosedExpr;
            break;
        }

        case TokenType::Semicolon:
        case TokenType::RightRoundBracket:
        {
            *terminationToken = token;
            if (plusFound)
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

