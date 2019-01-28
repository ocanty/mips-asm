//
// Created by ocanty on 28/01/19.
//

#ifndef MIPS_ASM_EMITTER_CONTEXT_HPP
#define MIPS_ASM_EMITTER_CONTEXT_HPP

#include <vector>
#include <map>
#include "../lexer/token.hpp"
#include "op_sequences.hpp"

namespace as {

class emitter_context {
public:
    explicit emitter_context(const token& tk) :
        m_token(tk) {
    }

    // .text section
    static const std::uint32_t SECTION_TEXT_BASE = 0x004000f0;

    /**
     * Get the latest token passed to the fsm
     * @return The token
     */
    const token& get_token() {
        return m_token;
    }

    /**
     * Set the token to be passed to the emitter when the fsm is ran
     * @param tk The token
     */
    void set_token(const token& tk) {
        m_token = tk;
    }

    /**
     * Get the token buffer)
     * @return
     */
    std::vector<token>& token_buffer() {
        return m_tokens_in;
    }

    void add_text_label(const std::string& label) {
        if(m_text_labels.count(label)) {
            std::cout << "warning: label redefinition, the new label will be used instead (" << label << ") " << std::endl;
        }

        m_text_labels.at(label) = SECTION_TEXT_BASE + m_text.size();
    }

    std::uint32_t get_text_label(const std::string& label) {
        return m_text_labels.at(label);
    }

    /**
     * Get the error to be shown if the next token did not match what was required
     * @return The error message
     */
    const std::string get_next_error() const {
        return m_next_error;
    }

    /**
     * Set the error to be shown if the next token does not match what is required
     * i.e. incorrect operands, types, etc...
     * @param err The error
     */
    void set_next_error(const std::string& err) {
        m_next_error = err;
    }

private:
    /* Current token */
    token m_token;


    std::string m_next_error;

    std::vector<token> m_tokens_in;

    std::vector<std::uint8_t> m_text;
    std::map<std::string, std::uint32_t> m_text_labels;
};
}

#endif //MIPS_ASM_EMITTER_CONTEXT_HPP
