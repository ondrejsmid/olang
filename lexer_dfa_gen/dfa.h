#include <vector>
#include <map>
#include <set>

/*
When two DFAs are being merged, and if it happens that for a certain
symbol they would both go to a final state, then exactly one of the DFAs'
final state will be consider final in the merged DFA.
This enum is to define, which one of the two DFAs will be the winner for providing
the 'finality' of a state in the case described. This priority needs to be defined for every final state of a DFA.
The reason is two prevent ambiguity for determining which token "type" the token should be:
for example when the lexer reads a token "if", is it a keyword, or a variable name?
*/
enum DfaUnionResolvePriority
{
    Keyword,
    Other
};

class Dfa
{
private:
    std::vector<std::vector<int>> transitions;
    std::map<int, DfaUnionResolvePriority> finalStates;
    bool isToStringValComputed;
    std::string toStringVal;
    DfaUnionResolvePriority dfaUnionResolvePriority;

public:
    Dfa(int statesCnt, std::map<int, DfaUnionResolvePriority> finalStates);
    void AddTransition(int srcState, int targetState, unsigned char c);
    void AddTransition(int srcState, int targetState, unsigned char intervalStartChar, unsigned char intervalEndChar);
    friend std::ostream & operator << (std::ostream &out, const Dfa &dfa);
};