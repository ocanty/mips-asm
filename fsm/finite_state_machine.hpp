//
// Created by ocanty on 15/01/19.
//

#ifndef MIPS_ASM_FINITE_STATE_MACHINE_HPP
#define MIPS_ASM_FINITE_STATE_MACHINE_HPP

#include <iostream>
#include <functional>
#include "transition_table.hpp"

using namespace std::placeholders;

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
    explicit finite_state_machine(States start_state) :
        m_current_state(start_state),
        m_on_no_transition_available(
            std::bind(&finite_state_machine<States,InputsType>::on_no_transition_available, this, _1))
    {

    }

    virtual ~finite_state_machine() = default;

    /**
     * Add a transition to the transition table
     * @param transition
     * @see transition
     */
    void add_transition(const transition<States,InputsType>& transition) {
        m_transition_table.add_transition(transition);
    }


    using no_transition_available_func = std::function<States(InputsType&)>;

    /**
     * Set the function that is called when no transition is available
     * @param func The function
     */
    void set_no_transition_available_handler(
        const no_transition_available_func& func) {
        m_on_no_transition_available = func;
    }

    /**
     * Add multiple transitions
     * @param transitions Vector of transitions
     */
    void add_transitions(const std::vector<transition<States,InputsType>>& transitions) {
        for(auto& transition : transitions) {
            add_transition(transition);
        }
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

        // if a transition occurred
        if(transition != std::nullopt) {

            // we need to run the callback, to run user-supplied state transition code
            auto callback = transition.value().get_transition_callback();
            callback(input);

            // update our state
            m_current_state = transition.value().get_transition_state();
        } else {
            // if no transition available,
            // ask our no transition available func what to do
            // (this just stays in the current state by default)
            m_current_state = m_on_no_transition_available(input);
        }

        return m_current_state;
    }

private:
    // if no transition is possible, just deadlock in current state
    States on_no_transition_available(const InputsType&) {
        return m_current_state;
    }

    // on_no_transition_available is typically bound onto this std::function
    // unless overridden by set_on_no_transition_available
    no_transition_available_func m_on_no_transition_available;

    States m_current_state;
    transition_table<States,InputsType> m_transition_table;

};

}

#endif //MIPS_ASM_FINITE_STATE_MACHINE_HPP
