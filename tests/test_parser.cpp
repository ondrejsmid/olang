#include <cassert>
#include "../parser/parser.h"

void AssertTrees(AstNode* expected, AstNode* actual)
{
    assert(expected != NULL);

    if (dynamic_cast<ProgramNode*>(expected) != NULL)
    {
        assert(dynamic_cast<ProgramNode*>(actual) != NULL);
        
        ProgramNode* expectedCasted = (ProgramNode*)(expected);
        ProgramNode* actualCasted = (ProgramNode*)(actual);
        
        assert(expectedCasted->statements.size() == actualCasted->statements.size());
        for (int i = 0; i < expectedCasted->statements.size(); i++)
        {
            AssertTrees(expectedCasted->statements[i], actualCasted->statements[i]);
        }
    }
    else if (dynamic_cast<AssignmentNode*>(expected) != NULL)
    {
        assert(dynamic_cast<AssignmentNode*>(actual) != NULL);

        AssignmentNode* expectedCasted = (AssignmentNode*)(expected);
        AssignmentNode* actualCasted = (AssignmentNode*)(actual);

        assert(expectedCasted->variableNameToken == actualCasted->variableNameToken);
        assert(expectedCasted->assignmentToken == actualCasted->assignmentToken);
        assert(expectedCasted->semicolonToken == actualCasted->semicolonToken);
        AssertTrees(expectedCasted->rightSideExpr, actualCasted->rightSideExpr);
    }
    else if (dynamic_cast<NumberNode*>(expected) != NULL)
    {
        assert(dynamic_cast<NumberNode*>(actual) != NULL);

        NumberNode* expectedCasted = (NumberNode*)(expected);
        NumberNode* actualCasted = (NumberNode*)(actual);
        
        assert(expectedCasted->numberToken == actualCasted->numberToken);
    }
}

void Parse_Program()
{
    char* text =
        " a = 5; "
        "b = 0 ; "
        " c =  -2; ";
    
    auto numberInAssig0 = new NumberNode();
    numberInAssig0->numberToken = Token(TokenType::Number, 5, 5);

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 1, 1);
    assignment0->assignmentToken = Token(TokenType::Assignment, 3, 3);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 6, 6);
    assignment0->rightSideExpr = numberInAssig0;

    auto numberInAssig1 = new NumberNode();
    numberInAssig1->numberToken = Token(TokenType::Number, 12, 12);

    auto assignment1 = new AssignmentNode();
    assignment1->variableNameToken = Token(TokenType::VariableName, 8, 8);
    assignment1->assignmentToken = Token(TokenType::Assignment, 10, 10);
    assignment1->semicolonToken = Token(TokenType::Semicolon, 14, 14);
    assignment1->rightSideExpr = numberInAssig1;

    auto numberInAssig2 = new NumberNode();
    numberInAssig2->numberToken = Token(TokenType::Number, 22, 23);

    auto assignment2 = new AssignmentNode();
    assignment2->variableNameToken = Token(TokenType::VariableName, 17, 17);
    assignment2->assignmentToken = Token(TokenType::Assignment, 19, 19);
    assignment2->semicolonToken = Token(TokenType::Semicolon, 24, 24);
    assignment2->rightSideExpr = numberInAssig2;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);
    expectedAst->statements.push_back(assignment1);
    expectedAst->statements.push_back(assignment2);

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    AssertTrees(expectedAst, actualAst);
    
    delete expectedAst;
    delete actualAst;
}

int main()
{
    AstNode* progNode = new ProgramNode();
    delete progNode;
    Parse_Program();
}