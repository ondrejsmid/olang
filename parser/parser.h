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

struct StringNode : ExprNode
{
    Token stringToken;
};

struct RightSideVariableNode : ExprNode
{
    Token variableNameToken;
};

struct AssocOperationNode : ExprNode
{
    std::vector<ExprNode*> operands;
    std::vector<Token> tokensBetweenOperands;
    ~AssocOperationNode();
};

struct EnclosedExpr : ExprNode
{
    Token leftRoundBracketToken;
    Token rightRoundBracketToken;
    ExprNode* innerExpr;
    ~EnclosedExpr();
};

struct UnaryMinusNode : EnclosedExpr
{
    Token unaryMinusToken;
};

struct AssignmentNode : AstNode
{
    Token variableNameToken;
    Token assignmentToken;
    Token semicolonToken;
    ExprNode* rightSideExpr;
    AssignmentNode();
    ~AssignmentNode();
};

struct IfNode : AstNode
{
    Token ifToken;
    Token leftRoundBracketToken;
    Token rightRoundBracketToken;
    Token leftCurlyBracketToken;
    Token rightCurlyBracketToken;
    ExprNode* condition;
    ProgramNode* programIfTrue;
    ~IfNode();
};

class Parser
{
public:
    Parser(char* text, size_t textLen);
    AstNode* Parse();


private:
    Lexer lexer;
    Token GetTokenThrowExceptionIfWrongType(TokenType tokenType);
    ProgramNode* ParseProgram(Token* terminationToken);
    AssignmentNode* ParseAssignment(const Token& variableNameToken);
    ExprNode* ParseExpr(Token* terminationToken);
    IfNode* ParseIf();
};

#endif