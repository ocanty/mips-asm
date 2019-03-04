//
// Created by ocanty on 28/01/19.
//

#ifndef MIPS_ASM_EMITTER_CONTEXT_HPP
#define MIPS_ASM_EMITTER_CONTEXT_HPP

#include <vector>
#include <sstream>
#include <map>
#include "../lexer/token.hpp"
#include "op_sequences.hpp"
#include "../spec/instruction_defs.hpp"


namespace as {

class emitter_context {
public:
    explicit emitter_context(const token& tk);

    // .text section base address
    static const std::uint32_t SECTION_TEXT_BASE = 0x004000f0;

    /**
     * Get the latest token passed to the fsm
     * @return The token
     */
    const token& cur_token() const;

    /**
     * Set the token to be passed to the emitter when the fsm is ran
     * @param tk The token
     */
    void set_cur_token(const token& tk);

    /**
     * Get the token buffer)
     * @return
     */
    std::vector<token>& token_buffer();

    std::vector<token_type> token_types_buffer() const;

    /**
     * Set the current text label, all instructions will be added into the memory block
     * at this point
     *
     * Note: you can not reuse labels
     * @param label
     */
    void set_current_text_label(const std::string& label);


    /**
     * Encode an instruction using the current token buffer and their attributes
     * @param sequence
     * @return MIPs instruction
     */
    std::optional<std::uint32_t> encode_instruction(const op_sequence& sequence);

    /**
     *
     * @param label
     * @return
     */

    std::uint32_t get_text_label(const std::string& label);


    /**
     * String stream for errors
     * @return Ref to error stringstream
     */
    std::stringstream& errors();

private:
    /* Current token */
    token m_token;


    std::stringstream m_errors;

    std::vector<token> m_tokens_in;

    std::vector<std::uint8_t> m_text;
    std::map<std::string, std::uint32_t> m_text_labels;
};
}

#endif //MIPS_ASM_EMITTER_CONTEXT_HPP
