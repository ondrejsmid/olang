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

AssocOperationNode::~AssocOperationNode()
{
    for (auto i = 0; i < operands.size(); i++)
    {
        delete operands[i];
    }
}

EnclosedExpr::~EnclosedExpr()
{
    delete innerExpr;
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

IfNode::IfNode()
    :
    elseNode(NULL)
{
}

IfNode::~IfNode()
{
    delete condition;
    delete programIfTrue;
    if (elseNode != NULL)
    {
        delete elseNode;
    }
}

ElseNode::~ElseNode()
{
    delete program;
}

Parser::Parser(char *text, size_t textLen)
    :
    lexer(text, textLen)
{
}

AstNode* Parser::Parse()
{
    Token dummyToken;
    return ParseProgram(&dummyToken);
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

ProgramNode *Parser::ParseProgram(Token* terminationToken)
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

            case TokenType::If:
            {
                auto ifNode = ParseIf(token);
                program->statements.push_back(ifNode);
                break;
            }
                
            case TokenType::RightCurlyBracket:
                *terminationToken = token;
                return program;

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

    bool isOperatorFound = false;
    TokenType foundOperator;

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

        case TokenType::True:
            {
            auto trueNode = new TrueNode();
            trueNode->trueToken = token;
            currentExprInAssocList = trueNode;
            break;
        }

        case TokenType::False:
        {
            auto falseNode = new FalseNode();
            falseNode->falseToken = token;
            currentExprInAssocList = falseNode;
            break;
        }

        case TokenType::Plus:
        case TokenType::And:
        case TokenType::Or:
            if (isOperatorFound)
            {
                if (token.tokenType != foundOperator)
                {
                    throw runtime_error("parse error");
                }
            }
            else
            {
                assocOperation = new AssocOperationNode();
                isOperatorFound = true;
                foundOperator = token.tokenType;
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

        case TokenType::Negate:
        {
            auto negate = new NegateNode();
            negate->negateToken = token;
            negate->leftRoundBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftRoundBracket);
            negate->innerExpr = ParseExpr(&(negate->rightRoundBracketToken));
            currentExprInAssocList = negate;
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
            if (isOperatorFound)
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

IfNode* Parser::ParseIf(const Token& ifToken)
{
    auto ifNode = new IfNode();
    ifNode->ifToken = ifToken;
    ifNode->leftRoundBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftRoundBracket);
    ifNode->condition = ParseExpr(&(ifNode->rightRoundBracketToken));
    ifNode->leftCurlyBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftCurlyBracket);
    ifNode->programIfTrue = ParseProgram(&(ifNode->rightCurlyBracketToken));

    auto token = lexer.GetNextNonWhitespaceToken();
    if (token.tokenType == TokenType::Else)
    {
        ifNode->elseNode = new ElseNode();
        ifNode->elseNode->elseToken = token;
        ifNode->elseNode->leftCurlyBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftCurlyBracket);
        ifNode->elseNode->program = ParseProgram(&(ifNode->elseNode->rightCurlyBracketToken));
    }
    else {
        lexer.MoveBack();
    }

    return ifNode;
}
