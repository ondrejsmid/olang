#include <string>
#include "dfa.h"

using namespace std;

Dfa::Dfa(int statesCnt, std::map<int, DfaUnionResolvePriority> finalStates)
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
