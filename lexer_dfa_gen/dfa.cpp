#include <string>
#include "dfa.h"

using namespace std;

Dfa::Dfa(int statesCnt, const map<int, DfaUnionResolvePriority> &finalStates)
    :
    finalStates(finalStates),
    dfaUnionResolvePriority(dfaUnionResolvePriority),
    isToStringValComputed(false), toStringVal("")
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
    for (char c = intervalStartChar; c <= intervalEndChar; c++)
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

            if (dfa.finalStates.find(srcState) != dfa.finalStates.end())
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
    map<pair<int, int>, DfaUnionResolvePriority> unionDfaFinalStates;
    
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

        bool isFinalInThisDfa = finalStates.find(statesIt->first) != finalStates.end();
        bool isFinalInOtherDfa = other.finalStates.find(statesIt->second) != other.finalStates.end();
        
        if (isFinalInThisDfa && isFinalInOtherDfa)
        {
            DfaUnionResolvePriority resolvePriorityInThisDfa = finalStates[statesIt->first];
            DfaUnionResolvePriority resolvePriorityInOtherDfa = other.finalStates[statesIt->second];
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
            DfaUnionResolvePriority resolvePriorityInThisDfa = finalStates[statesIt->first];
            unionDfaFinalStates[*statesIt] = resolvePriorityInThisDfa;
        } else if (isFinalInOtherDfa)
        {
            DfaUnionResolvePriority resolvePriorityInOtherDfa = other.finalStates[statesIt->second];
            unionDfaFinalStates[*statesIt] = resolvePriorityInOtherDfa;
        }

        unionStatesToResolve.erase(statesIt);
    }

    vector<vector<int>> unionTransitionsAsInts;
    
    return CreateDfaFromPairTypeStates(unionTransitions, unionDfaFinalStates);
}

Dfa Dfa::CreateDfaFromPairTypeStates(const map<pair<int, int>, map<char, pair<int, int>>> &unionTransitions,
    const map<pair<int, int>, DfaUnionResolvePriority> &unionDfaFinalStates)
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

    map<int, DfaUnionResolvePriority> finalStatesAsInts;

    for (map<pair<int, int>, DfaUnionResolvePriority>::const_iterator it = unionDfaFinalStates.begin(); it != unionDfaFinalStates.end(); ++it)
    {
        auto finalState = it->first;
        auto finalStatePriority = it->second;
        finalStatesAsInts[translationTable[finalState]] = finalStatePriority;
    }

    /* Translate transitions to int version. */

    Dfa unionDfa(unionTransitions.size(), finalStatesAsInts);

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

