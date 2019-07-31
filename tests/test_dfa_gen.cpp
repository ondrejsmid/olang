#include <cassert>
#include "../lexer_dfa_gen/lexer_dfa_gen.h"

void TestKeywordIsPreferredOverVariableName()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('i');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::VariableName);
    unionDfa.Move('f');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::If);
    unionDfa.Move('1');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::VariableName);
    unionDfa.Move('?');
    assert(unionDfa.IsInErrorState());
}

void Test_VariableName_ShallNotStartWithDigit()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('4');
    if (unionDfa.IsInFinalState())
    {
        assert(unionDfa.CurrentFinalStateTokenType() != TokenType::VariableName);
    }
}

void Test_Number_ZeroIsAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('0');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Test_Number_PositiveNumIsAccepted_1()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('1');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Test_Number_PositiveNumIsAccepted_12()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('1');
    unionDfa.Move('2');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Test_Number_NegativeNumIsAccepted_Minus7()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('-');
    unionDfa.Move('7');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Test_Number_NegativeNumIsAccepted_Minus78()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('-');
    unionDfa.Move('7');
    unionDfa.Move('8');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Test_Number_SingleDashIsNotAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('-');
    if (unionDfa.IsInFinalState())
    {
        assert(unionDfa.CurrentFinalStateTokenType() != TokenType::Number);
    }
}

void Test_Number_DashFollowedByZeroIsNotAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('-');
    unionDfa.Move('0');
    if (unionDfa.IsInFinalState())
    {
        assert(unionDfa.CurrentFinalStateTokenType() != TokenType::Number);
    }
}

int main()
{
    TestKeywordIsPreferredOverVariableName();
    Test_VariableName_ShallNotStartWithDigit();
    Test_Number_ZeroIsAccepted();
    Test_Number_PositiveNumIsAccepted_1();
    Test_Number_PositiveNumIsAccepted_12();
    Test_Number_NegativeNumIsAccepted_Minus7();
    Test_Number_NegativeNumIsAccepted_Minus78();
    Test_Number_DashFollowedByZeroIsNotAccepted();
}
