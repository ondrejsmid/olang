#ifndef DFA_H
#define DFA_H

#include <vector>
#include <map>
#include <set>

/*
Identifies a token type represented by a DFA.
Also serves when merging two DFAs:
When two DFAs are being merged, and if it happens that for a certain
symbol they would both go to a final state, then exactly one of those DFAs'
final states will be consider as final in the merged DFA.
An integer value of this enum will determine, which one of the two DFAs will be the winner for providing
the 'finality' of a state in the case described. This priority needs to be defined for every final state of a DFA.
As an example, consider that the lexer reads a token "if". Now, is it a keyword, or a variable name?
Note: The lower the enum value is in this source code, the higher is the priority.
*/
enum TokenType
{
    Invalid,
    Whitespace,
    VariableName,
    Number,
    Semicolon,
    Assign,
    Plus,
    If,
    RoundLeftBracket,
    RoundRightBracket,
};

class Dfa
{
private:
    std::vector<std::vector<int>> transitions;
    std::map<int, TokenType> finalStates;
    bool isToStringValComputed;
    std::string toStringVal;
    int currentState;

public:
    Dfa(){};
    Dfa(int statesCnt, const std::map<int, TokenType> &finalStates);

    void AddTransition(int srcState, int targetState, unsigned char c);

    void AddTransition(int srcState, int targetState, unsigned char intervalStartChar, unsigned char intervalEndChar);

    /* A pretty-print of the DFA in a way that transitions on chars that are ASCII-consequent
     * and that target the same target state,
     * are printed as a from-to interval instead of printing all the individual chars.
     * Thanks to this approach, the output is made far more compact and readable. */
    friend std::ostream & operator << (std::ostream &out, const Dfa &dfa);

    Dfa Union(Dfa &other);

    void Move(char c);

    bool IsInFinalState();

    bool IsInErrorState();

    bool GoesToErrorState(char c);

    void Reset();

    TokenType CurrentFinalStateTokenType();

    static Dfa CreateDfaFromPairTypeStates(const std::map<std::pair<int, int>, std::map<char, std::pair<int, int>>>& unionTransitions,
                                           const std::map<std::pair<int, int>, TokenType>& unionDfaFinalStates);
};

#endif  /* DFA_H */