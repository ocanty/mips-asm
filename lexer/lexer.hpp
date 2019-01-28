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

    /**
     * This is the class used for the inputs of the FSM
     */
    class lexer_data {
    public:
        /*
         * The current character that has been passed to the lexer
         * Note: m_char is the only variable the FSM depends on to determine state
         * The other variables are simple used for outputs
         *
         * Stored as an std::string to make passing to std::regex easier
         * */
        std::string m_char;

        std::size_t m_line;

        /* The output tokens */
        std::vector<token> m_tokens;

        /* Character buffer */
        std::stringstream m_buffer;

        void push_token(const token_type& type, const std::variant<std::string,std::int32_t>& attr = 0) {
            m_tokens.emplace_back(token(type, m_line, attr));
        }

        std::string get_buffer() {
            return m_buffer.str();
        }

        std::size_t get_line() {
            return m_line;
        }

        void clear_buffer() {
            m_buffer.clear(); // clear string
            m_buffer.str(""); // resets flags
        }
    };

    /*
     * Loads transitions into the FSM
     */
    void setup_fsm();

    finite_state_machine<states, lexer_data> m_fsm;
};

}
#endif //MIPS_ASM_LEXER_HPP
