//
// Created by ocanty on 16/01/19.
//

#ifndef MIPS_ASM_TRANSITION_TABLE_HPP
#define MIPS_ASM_TRANSITION_TABLE_HPP

#include <vector>
#include <map>
#include <optional>
#include "transition.hpp"

namespace as {

template <typename States, typename InputsType>
class transition_table {
public:
    transition_table() = default;

    /**
     * Add a transition to the transition table
     * @param transition
     */
    void add_transition(const transition<States, InputsType>& transition) {
        m_table.try_emplace(transition.get_initial_state(), transition);
    }

    /**
     * Test a state for transition against an input value
     * @param state The state
     * @param inputs The input type
     * @return An optional of transition
     */
    std::optional<transition<States, InputsType>>
    test_for_transitions(const States& state, const InputsType& inputs) {

        /* if the state has transitions */
        if(m_table.count(state)) {
            /* check each possible transition this state can undergo,
             * if one matches the correct condition (against the input value) it means a transition has to occur */
            for(auto& transition : m_table.at(state)) {
                if(transition.test_transition_contition(inputs)) {
                    return transition;
                }
            }
        }

        return std::nullopt;
    }

private:
    /**
     *  A map, which indexed by the current state, gives the possible transitions
     *  that the current state can undergo
     */
    std::map<States, std::vector<transition<States, InputsType>>> m_table;
};

}

#endif //MIPS_ASM_TRANSITION_TABLE_HPP
