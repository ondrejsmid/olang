#include <cassert>
#include <string>
#include "dfa.h"

using namespace std;

Dfa::Dfa(int statesCnt, const vector<pair<bool, TokenType>> &statesFinality)
    :
    statesFinality(statesFinality),
    isToStringValComputed(false),
    toStringVal(""),
    currentState(0)
{
    for (int i = 0; i < statesCnt; i++)
    {
        transitions.push_back(vector<int>(256, -1));
    }
}

void Dfa::AddTransition(int srcState, int targetState, unsigned char c)
{
    transitions[srcState][c] = targetState;
}

void Dfa::AddTransition(int srcState, int targetState, unsigned char intervalStartChar, unsigned char intervalEndChar)
{
    for (size_t c = intervalStartChar; c <= intervalEndChar; c++)
    {
        transitions[srcState][c] = targetState;
    }
}

ostream & operator << (ostream &out, const Dfa &dfa)
{
    if (!dfa.isToStringValComputed)
    {
        for (size_t srcState = 0; srcState < dfa.transitions.size(); srcState++)
        {
            out << "state " << srcState;

            if (dfa.statesFinality[srcState].first)
            {
                out << " (final)";
            }

            out << ":" << endl;

            vector<vector<vector<unsigned char>>> targetStatesTransitionCharSets(dfa.transitions.size());
            unsigned char currentIntervalLen = 0;

            for (size_t c = 0; c < 256; c++)
            {
                if (c == 255 || dfa.transitions[srcState][c] != dfa.transitions[srcState][c + 1])
                {
                    int currentTargetState = dfa.transitions[srcState][c];
                    
                    if (currentTargetState != -1) {
                        if (currentIntervalLen == 0)
                        {
                            targetStatesTransitionCharSets[currentTargetState].push_back(vector<unsigned char>{(unsigned char)c});
                        }
                        else {
                            targetStatesTransitionCharSets[currentTargetState].push_back(vector<unsigned char>{(unsigned char)((unsigned char)c - currentIntervalLen), (unsigned char)c});
                        }
                    }
                    currentIntervalLen = 0;
                }
                else {
                    currentIntervalLen ++;
                }
            }

            for (size_t targetState = 0; targetState < dfa.transitions.size(); targetState++)
            {
                if (targetStatesTransitionCharSets[targetState].size() == 0)
                {
                    continue;
                }
                out << srcState << " -> ";
                for (size_t i = 0; i < targetStatesTransitionCharSets[targetState].size(); i++)
                {
                    if (i != 0)
                    {
                        out << ",";
                    }
                    auto charInterval = targetStatesTransitionCharSets[targetState][i];
                    if (charInterval.size() == 1)
                    {
                        out << charInterval[0];
                    }
                    else {
                        out << charInterval[0] << "-" << charInterval[1];
                    }
                }
                out << " -> " << targetState << endl;
            }
            out << endl;
        }
    }

    out << dfa.toStringVal;
    return out;
}

Dfa Dfa::Union(Dfa &other)
{
    map<pair<int, int>, map<char, pair<int, int>>> unionTransitions;
    map<pair<int, int>, TokenType> unionDfaFinalStates;
    
    set<pair<int, int>> unionStatesToResolve;
    unionStatesToResolve.insert(pair<int, int>(0, 0));

    while (true) {
        auto statesIt = unionStatesToResolve.begin();
        if (statesIt == unionStatesToResolve.end())
        {
            break;
        }

        map<char, pair<int, int>> targetStatesInUnionDfaForCurrent;

        for (int c = 0; c < 256; c++)
        {
            int targetStateInThisDfa = statesIt->first == -1 ? -1 : transitions[statesIt->first][c];
            int targetStateInOtherDfa = statesIt->second == -1 ? -1 : other.transitions[statesIt->second][c];
            if (targetStateInThisDfa != -1 || targetStateInOtherDfa != -1)
            {
                auto targetStateInUnionDfa = pair<int, int>(targetStateInThisDfa, targetStateInOtherDfa);

                targetStatesInUnionDfaForCurrent[c] = targetStateInUnionDfa;

                if (unionTransitions.find(targetStateInUnionDfa) == unionTransitions.end())
                {
                    unionStatesToResolve.insert(targetStateInUnionDfa);
                }
            }
        }

        unionTransitions[*statesIt] = targetStatesInUnionDfaForCurrent;

		bool isFinalInThisDfa = IsStateFinal(statesIt->first);
		bool isFinalInOtherDfa = other.IsStateFinal(statesIt->second);
        
        if (isFinalInThisDfa && isFinalInOtherDfa)
        {
            TokenType resolvePriorityInThisDfa = FinalStateTokenType(statesIt->first);
            TokenType resolvePriorityInOtherDfa = other.FinalStateTokenType(statesIt->second);
            if (resolvePriorityInThisDfa > resolvePriorityInOtherDfa)
            {
                unionDfaFinalStates[*statesIt] = resolvePriorityInThisDfa;
            } else if (resolvePriorityInThisDfa < resolvePriorityInOtherDfa)
            {
                unionDfaFinalStates[*statesIt] = resolvePriorityInOtherDfa;
            } else
            {
                throw "Can't resolve DFA merge priority for a final state - same priorities found.";
            }
        } else if (isFinalInThisDfa)
        {
            TokenType resolvePriorityInThisDfa = FinalStateTokenType(statesIt->first);
            unionDfaFinalStates[*statesIt] = resolvePriorityInThisDfa;
        } else if (isFinalInOtherDfa)
        {
            TokenType resolvePriorityInOtherDfa = other.FinalStateTokenType(statesIt->second);
            unionDfaFinalStates[*statesIt] = resolvePriorityInOtherDfa;
        }

        unionStatesToResolve.erase(statesIt);
    }

    vector<vector<int>> unionTransitionsAsInts;
    
    return CreateDfaFromPairTypeStates(unionTransitions, unionDfaFinalStates);
}

void Dfa::Move(char c)
{
    currentState = transitions[currentState][c];
}

bool Dfa::IsStateFinal(size_t state)
{
	if (state == -1)
	{
		return false;
	}
	return statesFinality[state].first;
}

TokenType Dfa::FinalStateTokenType(size_t finalState)
{
	assert(IsStateFinal(finalState));

	return statesFinality[finalState].second;
}

bool Dfa::IsInFinalState()
{
	return IsStateFinal(currentState);
}

bool Dfa::IsInErrorState()
{
    return currentState == -1;
}

bool Dfa::GoesToErrorState(char c)
{
    return transitions[currentState][c] == -1;
}

TokenType Dfa::CurrentFinalStateTokenType()
{
    return FinalStateTokenType(currentState);
}

Dfa Dfa::CreateDfaFromPairTypeStates(const map<pair<int, int>, map<char, pair<int, int>>> &unionTransitions,
    const map<pair<int, int>, TokenType> &unionDfaFinalStatesAsPairs)
{
    /* Construct a translation table from int pairs to ints. */

    map<pair<int, int>, int> translationTable;
    translationTable[pair<int, int>(0, 0)] = 0;
    int lastIntUsed = 1;
    for (map<pair<int, int>, map<char, pair<int, int>>>::const_iterator srcStatesIt = unionTransitions.begin(); srcStatesIt != unionTransitions.end(); ++srcStatesIt)
    {
        auto srcState = srcStatesIt->first;
        if (srcState == pair<int, int>(0, 0))
        {
            continue;
        }
        translationTable[srcState] = lastIntUsed;
        lastIntUsed++;
    }

    /* Translate final states to ints. */

	vector<pair<bool, TokenType>> unionStatesFinality(unionTransitions.size(), pair<bool, TokenType>(false, TokenType::Invalid));

    for (map<pair<int, int>, TokenType>::const_iterator it = unionDfaFinalStatesAsPairs.begin(); it != unionDfaFinalStatesAsPairs.end(); ++it)
    {
        auto finalState = it->first;
        auto finalStatePriority = it->second;

		unionStatesFinality[translationTable[finalState]].first = true;
		unionStatesFinality[translationTable[finalState]].second = finalStatePriority;
    }

    /* Translate transitions to int version. */

    Dfa unionDfa(unionTransitions.size(), unionStatesFinality);

    for (map<pair<int, int>, map<char, pair<int, int>>>::const_iterator srcStatesIt = unionTransitions.begin(); srcStatesIt != unionTransitions.end(); ++srcStatesIt)
    {
        auto srcState = srcStatesIt->first;
        auto transitions = srcStatesIt->second;
        int srcStateAsInt = translationTable[srcState];
        for (map<char, pair<int, int>>::const_iterator transitionsIt = transitions.begin(); transitionsIt != transitions.end(); ++transitionsIt)
        {
            auto c = transitionsIt->first;
            auto targetState = transitionsIt->second;
            auto targetStateAsInt = translationTable[targetState];
            unionDfa.AddTransition(srcStateAsInt, targetStateAsInt, c);
        }
    }
    
    return unionDfa;
}

void Dfa::Reset()
{
    currentState = 0;
}