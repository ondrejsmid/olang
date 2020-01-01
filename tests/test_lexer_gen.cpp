#include <cassert>
#include "../lexer_gen/lexer_gen.h"

void Whitespace_SingleSpaceAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move(' ');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Whitespace);
}

void Whitespace_SpaceFollowebByTabAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move(' ');
    unionDfa.Move('\t');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Whitespace);
}

void Whitespace_LinuxNewlineAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('\n');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Whitespace);
}

void Whitespace_WindowsNewlineAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('\r');
    unionDfa.Move('\n');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Whitespace);
}

void KeywordIsPreferredOverVariableName()
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

void VariableName_ShallNotStartWithDigit()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('4');
    if (unionDfa.IsInFinalState())
    {
        assert(unionDfa.CurrentFinalStateTokenType() != TokenType::VariableName);
    }
}

void Number_ZeroIsAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('0');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Number_PositiveNumIsAccepted_1()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('1');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Number_PositiveNumIsAccepted_12()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('1');
    unionDfa.Move('2');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Number_NegativeNumIsAccepted_Minus7()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('-');
    unionDfa.Move('7');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Number_NegativeNumIsAccepted_Minus78()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('-');
    unionDfa.Move('7');
    unionDfa.Move('8');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Number);
}

void Number_SingleDashIsNotAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('-');
    if (unionDfa.IsInFinalState())
    {
        assert(unionDfa.CurrentFinalStateTokenType() != TokenType::Number);
    }
}

void Number_DashFollowedByZeroIsNotAccepted()
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

void SemicolonAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move(';');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Semicolon);
}

void AssignAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('=');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Assignment);
}

void PlusAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('+');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Plus);
}

void MinusAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('-');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::UnaryMinus);
}

void IfAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('i');
    unionDfa.Move('f');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::If);
}

void LeftRoundBracketAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('(');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::LeftRoundBracket);
}

void RightRoundBracketAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move(')');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::RightRoundBracket);
}

void InvalidOneCharWordNotAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('@');
    assert(unionDfa.IsInErrorState());
    assert(!unionDfa.IsInFinalState());
}

int main()
{
    Whitespace_SingleSpaceAccepted();
    Whitespace_SpaceFollowebByTabAccepted();
    Whitespace_LinuxNewlineAccepted();
    Whitespace_WindowsNewlineAccepted();
    KeywordIsPreferredOverVariableName();
    VariableName_ShallNotStartWithDigit();
    Number_ZeroIsAccepted();
    Number_PositiveNumIsAccepted_1();
    Number_PositiveNumIsAccepted_12();
    Number_NegativeNumIsAccepted_Minus7();
    Number_NegativeNumIsAccepted_Minus78();
    Number_DashFollowedByZeroIsNotAccepted();
    SemicolonAccepted();
    AssignAccepted();
    PlusAccepted();
    MinusAccepted();
    IfAccepted();
    LeftRoundBracketAccepted();
    RightRoundBracketAccepted();
    InvalidOneCharWordNotAccepted();
}
