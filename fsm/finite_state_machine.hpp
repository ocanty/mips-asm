//
// Created by ocanty on 15/01/19.
//

#ifndef MIPS_ASM_FINITE_STATE_MACHINE_HPP
#define MIPS_ASM_FINITE_STATE_MACHINE_HPP

#include <iostream>
#include <functional>
#include "transition_table.hpp"

namespace as {

/**
 * A finite state machine implementation
 * @tparam States       An enum of possible states
 * @tparam InputsType   The input that is passed to the machine when processing transitions
 *                      When you define transitions, you will be passed this input, and then you make the
 *                      decision to change state based on the passed input
 *                      (i.e. these trigger the state to change from one to the next)
 */
template <typename States, typename InputsType>
class finite_state_machine {
public:
    finite_state_machine(States start_state) :
        m_current_state(start_state) {

    }
    virtual ~finite_state_machine() = default;

//    Disabled: I need to re-learn argument forwarding :(
//    template <typename ...Args>
//    void add_transition(Args&&... t) {
//        m_transition_table.add_transition(
//                transition<States,InputsType>(std::forward<Args>(t)...)
//        );
//    }

    /**
     * Add a transition to the transition table
     * @param transition
     * @see transition
     */
    void add_transition(const transition<States,InputsType>& transition) {
        m_transition_table.add_transition(transition);
    }

    /**
     * Get the current state of the machine
     * @return state
     */
    States get_state() const {
        return m_current_state;
    }

    /**
     * Run the FSM for one input iteration,
     * @return The current state
     */
    States tick(InputsType& input) {
        auto transition = m_transition_table.test_for_transitions(m_current_state, input);

        // if a transition occured
        if(transition != std::nullopt) {

            // we need to run the callback, to run user-supplied state transition code
            auto callback = transition.value().get_transition_callback();
            callback(input);

            // update our state
            m_current_state = transition.value().get_transition_state();
        }

         return m_current_state;
    }

private:
    States m_current_state;
    transition_table<States,InputsType> m_transition_table;

};

}

#endif //MIPS_ASM_FINITE_STATE_MACHINE_HPP
