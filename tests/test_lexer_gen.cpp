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

void ElseAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('e');
    unionDfa.Move('l');
    unionDfa.Move('s');
    unionDfa.Move('e');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Else);
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

void LeftCurlyBracketAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('{');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::LeftCurlyBracket);
}

void RightCurlyBracketAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('}');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::RightCurlyBracket);
}

void TrueAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('t');
    unionDfa.Move('r');
    unionDfa.Move('u');
    unionDfa.Move('e');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::True);
}

void FalseAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('f');
    unionDfa.Move('a');
    unionDfa.Move('l');
    unionDfa.Move('s');
    unionDfa.Move('e');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::False);
}

void InvalidOneCharWordNotAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('@');
    assert(unionDfa.IsInErrorState());
    assert(!unionDfa.IsInFinalState());
}

void StringAccepted_TC1()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::String);
}

void StringAccepted_TC2()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('a');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::String);
}

void StringAccepted_TC3()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('a');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('@');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::String);
}

void StringAccepted_TC4()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::String);
}

void StringAccepted_TC5()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('a');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::String);
}

void StringAccepted_TC6()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::String);
}

void StringAccepted_TC7()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::String);
}

void StringAccepted_TC8()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::String);
}

void StringInvalidNotAccepted_TC1()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    unionDfa.Move('a');
    if (unionDfa.IsInFinalState())
    {
        assert(unionDfa.CurrentFinalStateTokenType() != TokenType::String);
    }
}

void StringInvalidNotAccepted_TC2()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
}

void StringInvalidNotAccepted_TC3()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('\\');
    assert(!unionDfa.IsInFinalState());
    unionDfa.Move('"');
    assert(!unionDfa.IsInFinalState());
}

void EqualAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('=');
    unionDfa.Move('=');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Equal);
}

void NegateAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('!');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Negate);
}

void NotEqualAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('!');
    unionDfa.Move('=');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::NotEqual);
}

void GreaterAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('>');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Greater);
}

void GreaterOrEqualAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('>');
    unionDfa.Move('=');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::GreaterOrEqual);
}

void LessAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('<');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Less);
}

void LessOrEqualAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('<');
    unionDfa.Move('=');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::LessOrEqual);
}

void AndAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('&');
    unionDfa.Move('&');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::And);
}

void OrAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('|');
    unionDfa.Move('|');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Or);
}

void MultiplyAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('*');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::Multiply);
}

void MultiplicationInversionAccepted()
{
    LexerGen lexer_gen;
    Dfa unionDfa = lexer_gen.unionDfa;
    unionDfa.Move('/');
    assert(unionDfa.IsInFinalState());
    assert(unionDfa.CurrentFinalStateTokenType() == TokenType::MultiplicationInversion);
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
    ElseAccepted();
    LeftRoundBracketAccepted();
    RightRoundBracketAccepted();
    LeftCurlyBracketAccepted();
    RightCurlyBracketAccepted();
    InvalidOneCharWordNotAccepted();
    StringAccepted_TC1();
    StringAccepted_TC2();
    StringAccepted_TC3();
    StringAccepted_TC4();
    StringAccepted_TC5();
    StringAccepted_TC6();
    StringAccepted_TC7();
    StringAccepted_TC8();
    StringInvalidNotAccepted_TC1();
    StringInvalidNotAccepted_TC2();
    StringInvalidNotAccepted_TC3();
    TrueAccepted();
    FalseAccepted();
    EqualAccepted();
    NegateAccepted();
    NotEqualAccepted();
    GreaterAccepted();
    GreaterOrEqualAccepted();
    LessAccepted();
    LessOrEqualAccepted();
    AndAccepted();
    OrAccepted();
    MultiplyAccepted();
    MultiplicationInversionAccepted();
}
