#include "../lexer/lexer.h"

struct AstNode
{
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

struct AssignmentNode : AstNode
{
    Token variableNameToken;
    Token assignmentToken;
    Token semicolonToken;
    ExprNode * rightSideExpr;
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
    ExprNode * ParseExpr();
};
