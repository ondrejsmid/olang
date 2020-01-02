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
    else if (dynamic_cast<StringNode*>(expected) != NULL)
    {
        assert(dynamic_cast<StringNode*>(actual) != NULL);

        StringNode* expectedCasted = (StringNode*)(expected);
        StringNode* actualCasted = (StringNode*)(actual);

        assert(expectedCasted->stringToken == actualCasted->stringToken);
    }
    else if (dynamic_cast<RightSideVariableNode*>(expected) != NULL)
    {
        assert(dynamic_cast<RightSideVariableNode*>(actual) != NULL);

        RightSideVariableNode* expectedCasted = (RightSideVariableNode*)(expected);
        RightSideVariableNode* actualCasted = (RightSideVariableNode*)(actual);

        assert(expectedCasted->variableNameToken == actualCasted->variableNameToken);
    }
    else if (dynamic_cast<AssocOperationNode*>(expected) != NULL)
    {
        assert(dynamic_cast<AssocOperationNode*>(actual) != NULL);

        AssocOperationNode* expectedCasted = (AssocOperationNode*)(expected);
        AssocOperationNode* actualCasted = (AssocOperationNode*)(actual);

        assert(std::equal(expectedCasted->tokensBetweenOperands.begin(), expectedCasted->tokensBetweenOperands.end(), actualCasted->tokensBetweenOperands.begin()));

        assert(expectedCasted->operands.size() == actualCasted->operands.size());
        auto expectedOperand = expectedCasted->operands.begin();
        auto actualOperand = actualCasted->operands.begin();
        while (expectedOperand != expectedCasted->operands.end())
        {
            AssertTrees(*expectedOperand, *actualOperand);
            ++expectedOperand;
            ++actualOperand;
        }
    }
    else if (dynamic_cast<EnclosedExpr*>(expected) != NULL)
    {
        assert(dynamic_cast<EnclosedExpr*>(actual) != NULL);

        EnclosedExpr* expectedCasted = (EnclosedExpr*)(expected);
        EnclosedExpr* actualCasted = (EnclosedExpr*)(actual);

        assert(expectedCasted->leftRoundBracketToken == actualCasted->leftRoundBracketToken);
        assert(expectedCasted->rightRoundBracketToken == actualCasted->rightRoundBracketToken);
        AssertTrees(expectedCasted->innerExpr, actualCasted->innerExpr);
    }
    else if (dynamic_cast<UnaryMinusNode*>(expected) != NULL)
    {
        assert(dynamic_cast<UnaryMinusNode*>(actual) != NULL);

        UnaryMinusNode* expectedCasted = (UnaryMinusNode*)(expected);
        UnaryMinusNode* actualCasted = (UnaryMinusNode*)(actual);

        assert(expectedCasted->leftRoundBracketToken == actualCasted->leftRoundBracketToken);
        assert(expectedCasted->rightRoundBracketToken == actualCasted->rightRoundBracketToken);
        assert(expectedCasted->unaryMinusToken == actualCasted->unaryMinusToken);
        AssertTrees(expectedCasted->innerExpr, actualCasted->innerExpr);
    }
    else {
        assert(!"Not yet supported AST node types found.");
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

void Parse_AssignmentOfRightSideVariable()
{
    char* text = "a = b;";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto rightSideVariable = new RightSideVariableNode();
    rightSideVariable->variableNameToken = Token(TokenType::VariableName, 4, 4);

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 5, 5);
    assignment0->rightSideExpr = rightSideVariable;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete expectedAst;
    delete actualAst;
}

void Parse_AssignmentOfAssocOperation_Addition_TC1()
{
    char* text = "a = b + 2;";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();
    
    auto rightSideVariable = new RightSideVariableNode();
    rightSideVariable->variableNameToken = Token(TokenType::VariableName, 4, 4);

    auto number = new NumberNode();
    number->numberToken = Token(TokenType::Number, 8, 8);

    auto assocOperation = new AssocOperationNode();
    assocOperation->operands.push_back(rightSideVariable);
    assocOperation->operands.push_back(number);
    assocOperation->tokensBetweenOperands.push_back(Token(TokenType::Plus, 6, 6));

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 9, 9);
    assignment0->rightSideExpr = assocOperation;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete actualAst;
}

void Parse_AssignmentOfAssocOperation_Addition_TC2()
{
    char* text = "a = b + 2 + c;";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto rightSideVariable0 = new RightSideVariableNode();
    rightSideVariable0->variableNameToken = Token(TokenType::VariableName, 4, 4);

    auto number = new NumberNode();
    number->numberToken = Token(TokenType::Number, 8, 8);

    auto rightSideVariable1 = new RightSideVariableNode();
    rightSideVariable1->variableNameToken = Token(TokenType::VariableName, 12, 12);

    auto assocOperation = new AssocOperationNode();
    assocOperation->operands.push_back(rightSideVariable0);
    assocOperation->operands.push_back(number);
    assocOperation->operands.push_back(rightSideVariable1);
    assocOperation->tokensBetweenOperands.push_back(Token(TokenType::Plus, 6, 6));
    assocOperation->tokensBetweenOperands.push_back(Token(TokenType::Plus, 10, 10));

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 13, 13);
    assignment0->rightSideExpr = assocOperation;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete actualAst;
}

void Parse_AssignmentOfEnclosedExpr()
{
    char* text = "a = (b);";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto rightSideVariable = new RightSideVariableNode();
    rightSideVariable->variableNameToken = Token(TokenType::VariableName, 5, 5);

    auto enclosedExpr = new EnclosedExpr();
    enclosedExpr->leftRoundBracketToken = Token(TokenType::LeftRoundBracket, 4, 4);
    enclosedExpr->rightRoundBracketToken = Token(TokenType::RightRoundBracket, 6, 6);
    enclosedExpr->innerExpr = rightSideVariable;

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 7, 7);
    assignment0->rightSideExpr = enclosedExpr;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete actualAst;
}

void Parse_AssignmentOfUnaryMinus_TC1()
{
    char* text = "a = -(b);";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto rightSideVariable = new RightSideVariableNode();
    rightSideVariable->variableNameToken = Token(TokenType::VariableName, 6, 6);

    auto unaryMinus = new UnaryMinusNode();
    unaryMinus->unaryMinusToken = Token(TokenType::UnaryMinus, 4, 4);
    unaryMinus->leftRoundBracketToken = Token(TokenType::LeftRoundBracket, 5, 5);
    unaryMinus->rightRoundBracketToken = Token(TokenType::RightRoundBracket, 7, 7);
    unaryMinus->innerExpr = rightSideVariable;

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 8, 8);
    assignment0->rightSideExpr = unaryMinus;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete actualAst;
}

void Parse_AssignmentOfUnaryMinus_TC2()
{
    char* text = "a = -(b + 2);";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto rightSideVariable = new RightSideVariableNode();
    rightSideVariable->variableNameToken = Token(TokenType::VariableName, 6, 6);

    auto number = new NumberNode();
    number->numberToken = Token(TokenType::Number, 10, 10);

    auto assocOperation = new AssocOperationNode();
    assocOperation->operands.push_back(rightSideVariable);
    assocOperation->operands.push_back(number);
    assocOperation->tokensBetweenOperands.push_back(Token(TokenType::Plus, 8, 8));

    auto unaryMinus = new UnaryMinusNode();
    unaryMinus->unaryMinusToken = Token(TokenType::UnaryMinus, 4, 4);
    unaryMinus->leftRoundBracketToken = Token(TokenType::LeftRoundBracket, 5, 5);
    unaryMinus->rightRoundBracketToken = Token(TokenType::RightRoundBracket, 11, 11);
    unaryMinus->innerExpr = assocOperation;

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 12, 12);
    assignment0->rightSideExpr = unaryMinus;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete actualAst;
}

void Parse_AssignmentOfUnaryMinus_TC3()
{
    char* text = "a = -(b + 2) + c + d;";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto rightSideVariable0 = new RightSideVariableNode();
    rightSideVariable0->variableNameToken = Token(TokenType::VariableName, 6, 6);

    auto number = new NumberNode();
    number->numberToken = Token(TokenType::Number, 10, 10);

    auto assocOperation0 = new AssocOperationNode();
    assocOperation0->operands.push_back(rightSideVariable0);
    assocOperation0->operands.push_back(number);
    assocOperation0->tokensBetweenOperands.push_back(Token(TokenType::Plus, 8, 8));

    auto unaryMinus = new UnaryMinusNode();
    unaryMinus->unaryMinusToken = Token(TokenType::UnaryMinus, 4, 4);
    unaryMinus->leftRoundBracketToken = Token(TokenType::LeftRoundBracket, 5, 5);
    unaryMinus->rightRoundBracketToken = Token(TokenType::RightRoundBracket, 11, 11);
    unaryMinus->innerExpr = assocOperation0;

    auto rightSideVariable1 = new RightSideVariableNode();
    rightSideVariable1->variableNameToken = Token(TokenType::VariableName, 15, 15);

    auto rightSideVariable2 = new RightSideVariableNode();
    rightSideVariable2->variableNameToken = Token(TokenType::VariableName, 19, 19);

    auto assocOperation1 = new AssocOperationNode();
    assocOperation1->operands.push_back(unaryMinus);
    assocOperation1->operands.push_back(rightSideVariable1);
    assocOperation1->operands.push_back(rightSideVariable2);
    assocOperation1->tokensBetweenOperands.push_back(Token(TokenType::Plus, 13, 13));
    assocOperation1->tokensBetweenOperands.push_back(Token(TokenType::Plus, 17, 17));

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 20, 20);
    assignment0->rightSideExpr = assocOperation1;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete actualAst;
}

void Parse_AssignmentOfString_TC1()
{
    char* text = "a = \"gh@-\";";
    // A corresponding Olang code is literally this: a = "gh@-";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto stringNode = new StringNode();
    stringNode->stringToken = Token(TokenType::String, 4, 9);

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 10, 10);
    assignment0->rightSideExpr = stringNode;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete expectedAst;
    delete actualAst;
}

void Parse_AssignmentOfAssocOperationWithStringAndString()
{
    char* text = "a = \"She said \" + \"\\\"Hi!\\\"\";";
    // A corresponding Olang code is literally this: a = "She said " + "\"Hi!\"";

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto stringNode0 = new StringNode();
    stringNode0->stringToken = Token(TokenType::String, 4, 14);

    auto stringNode1 = new StringNode();
    stringNode1->stringToken = Token(TokenType::String, 18, 26);

    auto assocOperation = new AssocOperationNode();
    assocOperation->operands.push_back(stringNode0);
    assocOperation->operands.push_back(stringNode1);
    assocOperation->tokensBetweenOperands.push_back(Token(TokenType::Plus, 16, 16));

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 27, 27);
    assignment0->rightSideExpr = assocOperation;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete expectedAst;
    delete actualAst;
}

void Parse_AssignmentOfAssocOperationWithStringAndNumber()
{
    char* text = "a = \"I like number \" + 7;";
    // A corresponding Olang code is literally this: a = "I like number " + 7;

    Parser parser(text, strlen(text));
    auto actualAst = parser.Parse();

    auto stringNode = new StringNode();
    stringNode->stringToken = Token(TokenType::String, 4, 19);

    auto number = new NumberNode();
    number->numberToken = Token(TokenType::Number, 23, 23);

    auto assocOperation = new AssocOperationNode();
    assocOperation->operands.push_back(stringNode);
    assocOperation->operands.push_back(number);
    assocOperation->tokensBetweenOperands.push_back(Token(TokenType::Plus, 21, 21));

    auto assignment0 = new AssignmentNode();
    assignment0->variableNameToken = Token(TokenType::VariableName, 0, 0);
    assignment0->assignmentToken = Token(TokenType::Assignment, 2, 2);
    assignment0->semicolonToken = Token(TokenType::Semicolon, 24, 24);
    assignment0->rightSideExpr = assocOperation;

    auto expectedAst = new ProgramNode();
    expectedAst->statements.push_back(assignment0);

    AssertTrees(expectedAst, actualAst);

    delete expectedAst;
    delete actualAst;
}

int main()
{
    Parse_Program();
    Parse_AssignmentOfRightSideVariable();
    Parse_AssignmentOfAssocOperation_Addition_TC1();
    Parse_AssignmentOfAssocOperation_Addition_TC2();
    Parse_AssignmentOfEnclosedExpr();
    Parse_AssignmentOfUnaryMinus_TC1();
    Parse_AssignmentOfUnaryMinus_TC2();
    Parse_AssignmentOfUnaryMinus_TC3();
    Parse_AssignmentOfString_TC1();
    Parse_AssignmentOfAssocOperationWithStringAndString();
    Parse_AssignmentOfAssocOperationWithStringAndNumber();
}