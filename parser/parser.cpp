#include "parser.h"
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
            
            case TokenType::While:
            {
                auto whileNode = ParseWhile(token);
                program->statements.push_back(whileNode);
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

ExprNode * Parser::ParseExpr(Token* terminationToken)
{
    AssocOperationNode* assocOperation = new AssocOperationNode();

    while (true)
    {
        auto token = lexer.GetNextNonWhitespaceToken();
        bool isTokenOperator = false;
        switch (token.tokenType)
        {
        case TokenType::Number:
        {
            auto number = new NumberNode();
            number->numberToken = token;
            assocOperation->operands.push_back(number);
            break;
        }

        case TokenType::VariableName:
        {
            auto rightSideVariable = new RightSideVariableNode();
            rightSideVariable->variableNameToken = token;
            assocOperation->operands.push_back(rightSideVariable);
            break;
        }

        case TokenType::String:
        {
            auto stringNode = new StringNode();
            stringNode->stringToken = token;
            assocOperation->operands.push_back(stringNode);
            break;
        }

        case TokenType::True:
            {
            auto trueNode = new TrueNode();
            trueNode->trueToken = token;
            assocOperation->operands.push_back(trueNode);
            break;
        }

        case TokenType::False:
        {
            auto falseNode = new FalseNode();
            falseNode->falseToken = token;
            assocOperation->operands.push_back(falseNode);
            break;
        }

        /* binary operators (i.e. exactly 1 operator token) */
        case TokenType::Equal:
        case TokenType::NotEqual:
        case TokenType::Greater:
        case TokenType::GreaterOrEqual:
        case TokenType::Less:
        case TokenType::LessOrEqual:
            if (assocOperation->operatorTokens.size() == 1)
            {
                throw runtime_error("Parse error: a binary assoc list must not have more than 1 operator token.");
            }
            isTokenOperator = true;
            break;

        /* n-ary operators, where n >= 3 (i.e. operator tokens cnt >= 2) */
        case TokenType::Plus:
        case TokenType::Multiply:
        case TokenType::And:
        case TokenType::Or:
            if (assocOperation->operatorTokens.size() >= 1 && token.tokenType != assocOperation->operatorTokens.front().tokenType)
            {
                throw runtime_error("Parse error: different operators must not be present in the same assoc list.");
            }
            isTokenOperator = true;
            break;

        case TokenType::UnaryMinus:
        {
            auto unaryMinus = new UnaryMinusNode();
            unaryMinus->unaryMinusToken = token;
            unaryMinus->leftRoundBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftRoundBracket);
            unaryMinus->innerExpr = ParseExpr(&(unaryMinus->rightRoundBracketToken));
            assocOperation->operands.push_back(unaryMinus);
            break;
        }

        case TokenType::MultiplicationInversion:
        {
            auto multiplicationInversion = new MultiplicationInversionNode();
            multiplicationInversion->multiplicationInversionToken = token;
            multiplicationInversion->leftRoundBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftRoundBracket);
            multiplicationInversion->innerExpr = ParseExpr(&(multiplicationInversion->rightRoundBracketToken));
            assocOperation->operands.push_back(multiplicationInversion);
            break;
        }

        case TokenType::Negate:
        {
            auto negate = new NegateNode();
            negate->negateToken = token;
            negate->leftRoundBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftRoundBracket);
            negate->innerExpr = ParseExpr(&(negate->rightRoundBracketToken));
            assocOperation->operands.push_back(negate);
            break;
        }

        case TokenType::LeftRoundBracket:
        {
            auto enclosedExpr = new EnclosedExpr();
            enclosedExpr->leftRoundBracketToken = token;
            enclosedExpr->innerExpr = ParseExpr(&(enclosedExpr->rightRoundBracketToken));
            assocOperation->operands.push_back(enclosedExpr);
            break;
        }

        /* termination */
        case TokenType::Semicolon:
        case TokenType::RightRoundBracket:
        {
            *terminationToken = token;
            if (assocOperation->operands.size() > 1)
            {
                return assocOperation;
            }
            else {
                return assocOperation->operands[0];
            }
        }

        default:
            throw runtime_error("parse error");
        }

        if (isTokenOperator)
        {
            assocOperation->operatorTokens.push_back(token);
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

WhileNode* Parser::ParseWhile(const Token& whileToken)
{
    auto whileNode = new WhileNode();
    whileNode->whileToken = whileToken;
    whileNode->leftRoundBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftRoundBracket);
    whileNode->condition = ParseExpr(&(whileNode->rightRoundBracketToken));
    whileNode->leftCurlyBracketToken = GetTokenThrowExceptionIfWrongType(TokenType::LeftCurlyBracket);
    whileNode->program = ParseProgram(&(whileNode->rightCurlyBracketToken));
    return whileNode;
}
