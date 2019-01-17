//
// Created by ocanty on 15/01/19.
//

#ifndef MIPS_ASM_FINITE_STATE_MACHINE_HPP
#define MIPS_ASM_FINITE_STATE_MACHINE_HPP

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
    finite_state_machine() = default;
    virtual ~finite_state_machine() = default;

    transition_table<States,InputsType>& transition_table() {
        return m_transition_table;
    }

private:
    ::as::transition_table<States,InputsType> m_transition_table;

};

}

#endif //MIPS_ASM_FINITE_STATE_MACHINE_HPP
