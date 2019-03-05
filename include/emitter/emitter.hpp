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

    enum base_states : std::size_t {
        // Base state
        INITIAL_STATE = 0,

        // We have passed a .data directive and are in data mode
        DATA_STATE,

        // We have passed a .text directive and are in text mode
        TEXT_STATE,

        // From TEXT_STATE, seek until we encounter a newline delimiter
        TEXT_SEEK_UNTIL_NEWLINE
    };

    void setup_fsm();
};


}

#endif // MIPS_ASM_EMITTER_HPP
