//
// Created by ocanty on 25/01/19.
//

#ifndef MIPS_ASM_EMITTER_HPP
#define MIPS_ASM_EMITTER_HPP

#include "../fsm/finite_state_machine.hpp"
#include "../lexer/token.hpp"
#include "emitter_context.hpp"
#include "op_sequences.hpp"
#include <list>
#include <functional>

namespace as {


class emitter {
public:
    emitter();
    virtual ~emitter() = default;

    std::optional<std::vector<std::uint8_t>> emit(const std::vector<token>& tokens);

private:
    finite_state_machine<std::size_t, emitter_context> m_fsm;

    /**
     * We are generating states at runtime,
     * these are the additional ones we require
     */
    enum base_states : std::size_t {
        // Base state
        INITIAL_STATE = 0,

        // We have passed a .data directive and are in data mode
        DATA_STATE = 1,

        // We have passed a .text directive and are in text mode
        TEXT_STATE = 2,

        // Compile error occured
        ERROR_STATE = 3,

        // Use to get next free state @ runtime
        _LAST_BASE_STATE
    };

    // The next state number we can add to the fsm
    // i.e this is the next free state
    std::size_t m_last_state = _LAST_BASE_STATE;

    void add_transitions_for_sequence(
            const std::size_t& start_state,
            const op_sequence& sequence,
            std::function<void(emitter_context&)> handler);

    void setup_fsm();
};


}

#endif // MIPS_ASM_EMITTER_HPP
