//
// Created by ocanty on 16/01/19.
//

#ifndef MIPS_ASM_LEXER_HPP
#define MIPS_ASM_LEXER_HPP

#include <cstdint>
#include <variant>
#include <sstream>

#include "../fsm/finite_state_machine.hpp"
#include "../fsm/transition.hpp"


#include "lexer_context.hpp"
#include "token.hpp"

namespace as {

class lexer {
public:
    lexer();
    virtual ~lexer() = default;

    /**
     * Convert a string of MIPS assembly into tokens
     * @returns vector of token
     */
    std::optional<std::vector<token>> lex(const std::string& input);

private:
    /**
     * The possible states the lexer's FSM can be in
     */
    enum class states {
        // Base state, no input has occurred yet or we have returned from a token push
        BASE,

        // We have entered a directive, continue to eat characters till a space, push the token then go to BASE
        SEEK_DIRECTIVE,

        // Like SEEK_DIRECTIVE, but for labels
        SEEK_LABEL_OR_MNEMONIC,

        // Seek till new line or EOF, then return to base
        SEEK_COMMENT,

        // $rs
        SEEK_REGISTER,

        // Literal number
        SEEK_LITERAL_NUMBER,

        // Literal character
        SEEK_LITERAL_CHAR,

        // Literal string,
        SEEK_LITERAL_STRING,

        // IMM($reg)
        //    _^
        // from literal
        SEEK_IMM_REG_PRE,

        // IMM($reg)
        //      ^^^
        SEEK_IMM_REG,

        // An unexpected, unknown token
        INVALID_TOKEN = -1
    };

    /*
     * Loads transitions into the FSM
     */
    void setup_fsm();

    finite_state_machine<states, lexer_context> m_fsm;
};

}
#endif //MIPS_ASM_LEXER_HPP
