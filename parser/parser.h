#include "../lexer/lexer.h"

#ifndef PARSER_H
#define PARSER_H

struct AstNode
{
    virtual ~AstNode() {};
};

struct ProgramNode : AstNode
{
    std::vector<AstNode*> statements;
    ~ProgramNode();
};

struct ExprNode : AstNode
{
};

struct NumberNode : ExprNode
{
    Token numberToken;
};

struct RightSideVariableNode : ExprNode
{
    Token variableNameToken;
};

struct AssocOperationNode : ExprNode
{
    std::vector<ExprNode*> operands;
    std::vector<Token> tokensBetweenOperands;
};

struct AssignmentNode : AstNode
{
    Token variableNameToken;
    Token assignmentToken;
    Token semicolonToken;
    ExprNode * rightSideExpr;
    AssignmentNode();
    ~AssignmentNode();
};

class Parser
{
public:
    Parser(char *text, size_t textLen);
    ProgramNode *Parse();

private:
    Lexer lexer;
    Token GetTokenThrowExceptionIfWrongType(TokenType tokenType);
    AssignmentNode * ParseAssignment(const Token & variableNameToken);
    ExprNode * ParseExpr(Token* semicolonToken);
};

#endif