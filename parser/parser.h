#include <cstring>
#include <cassert>
#include "../lexer/lexer.h"

#ifndef PARSER_H
#define PARSER_H

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

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

struct TrueNode : ExprNode
{
    Token trueToken;
};

struct FalseNode : ExprNode
{
    Token falseToken;
};

struct RightSideVariableNode : ExprNode
{
    Token variableNameToken;
};

struct AssocOperationNode : ExprNode
{
    std::vector<ExprNode*> operands;
    std::vector<Token> operatorTokens;
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

struct MultiplicationInversionNode : EnclosedExpr
{
    Token multiplicationInversionToken;
};

struct NegateNode : EnclosedExpr
{
    Token negateToken;
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

struct ElseNode : AstNode
{
    Token elseToken;
    Token leftCurlyBracketToken;
    Token rightCurlyBracketToken;
    ProgramNode* program;
    ~ElseNode();
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
    ElseNode* elseNode;
    IfNode();
    ~IfNode();
};

struct WhileNode : AstNode
{
    Token whileToken;
    Token leftRoundBracketToken;
    Token rightRoundBracketToken;
    Token leftCurlyBracketToken;
    Token rightCurlyBracketToken;
    ExprNode* condition;
    ProgramNode* program;
};

struct DeclarationNode : AstNode
{
    Token varKwToken;
    Token variableNameToken;
    Token semicolonToken;
};

class Parser
{
public:
    Parser(char* text, size_t textLen);
    AstNode* Parse();

private:
    Lexer lexer;
    Token GetTokenThrowExceptionIfWrongType(TokenType expectedTokenType);
    void ParseError(std::string msg);
    ProgramNode* ParseProgram(Token* terminationToken);
    AssignmentNode* ParseAssignment(const Token& variableNameToken);
    ExprNode* ParseExpr(Token* terminationToken);
    IfNode* ParseIf(const Token& ifToken);
    WhileNode* ParseWhile(const Token& whileToken);
};

#endif