//
// Created by ocanty on 06/02/19.
//

#ifndef MIPS_ASM_LEXER_CONTEXT_HPP
#define MIPS_ASM_LEXER_CONTEXT_HPP

#include <string>
#include <vector>
#include "token.hpp"
#include <sstream>
#include <variant>

namespace as {

/**
 * This is the class used for the inputs and outputs of the lexer FSM
 * A reference to this class should be passed to every transition within the FSM
 * This is required to calculate the next possible state
 * based on the input character
 */
class lexer_context {
public:
    /**
     * Get the character that has been passed as an input to the fsm
     * @return
     */
    const char& ch() {
        return m_char;
    }

    /**
     * Set the char to be passed
     * @param ch
     */
    void set_ch(const char& ch) {
        m_char = ch;
    }

    /**
     * Get current line
     * @return current line
     */
    const std::size_t& cur_line() {
        return m_line;
    }

    /**
     * Set the current line
     * @param line
     */
    void set_cur_line(const size_t& line) {
        m_line = line;
    }

    /**
     * Consume a character into the character buffer
     * @param ch character
     */
     void consume(const char& ch) {
         m_buffer << ch;
     }

    /**
     * Return the character buffer
     * @return
     */
    const std::stringstream& char_buffer() const {
        return m_buffer;
    }

    /**
     * Clear character buffer, does not clear hte current character
     */
    void clear_char_buffer() {
        m_buffer.clear(); // clear string
        m_buffer.str(""); // resets flags
    }


    /* The output tokens */
    const std::vector<token>& tokens() const {
        return m_tokens;
    }

    /**
     * Add a token to the output token buffer
     * @param type  Token type
     * @param attr  Token attribute
     */
    void push_token(const token_type &type, const std::variant<std::string, std::int32_t> &attr = 0) {
        m_tokens.emplace_back(token(type, m_line, attr));
    }

private:
    /*
     * The current character that has been passed to the lexer
     * Note: m_char is the only variable the lexer FSM depends on to determine state
     * The other variables are simple used for outputs
     *
     * Stored as an std::string to make passing to std::regex easier
     **/
    char m_char;
    std::vector<token> m_tokens;

    /* Character buffer */
    std::stringstream m_buffer;

    /* current line */
    std::size_t m_line;

};

}

#endif //MIPS_ASM_LEXER_CONTEXT_HPP
