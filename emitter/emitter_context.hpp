//
// Created by ocanty on 28/01/19.
//

#ifndef MIPS_ASM_EMITTER_CONTEXT_HPP
#define MIPS_ASM_EMITTER_CONTEXT_HPP

#include <vector>
#include <map>
#include "../lexer/token.hpp"
#include "op_sequences.hpp"
#include "../spec/instruction_defs.hpp"


namespace as {

class emitter_context {
public:
    explicit emitter_context(const token& tk) :
        m_token(tk) {
    }

    // .text section base address
    static const std::uint32_t SECTION_TEXT_BASE = 0x004000f0;

    /**
     * Get the latest token passed to the fsm
     * @return The token
     */
    const token& cur_token() const {
        return m_token;
    }

    /**
     * Set the token to be passed to the emitter when the fsm is ran
     * @param tk The token
     */
    void set_cur_token(const token& tk) {
        m_token = tk;
    }

    /**
     * Get the token buffer)
     * @return
     */
    std::vector<token>& token_buffer() {
        return m_tokens_in;
    }

    /**
     * Set the current text label, all instructions will be added into the memory block
     * at this point
     *
     * Note: you can not reuse labels
     * @param label
     */
    void set_current_text_label(const std::string& label) {
        if(m_text_labels.count(label)) {
            std::cout << "warning: label redefinition, the new label will be used instead (" << label << ") " << std::endl;
        }

        m_text_labels.at(label) = SECTION_TEXT_BASE + m_text.size();
    }

    /**
     * Write a binary instruction to .text
     * This uses the current token_buffer
     * @param ins   The
     * @param data
     */
    bool try_write_instruction(const op_sequence& sequence) {
        auto mnemonic_pos = sequence.position_of("mnemonic");
//
//        if(mnemonic_pos != std::nullopt) {
//            // This should never happen
//            std::cout << "Could not determine the position of mneomic" << std::endl;
//
//            return false;
//        }

        auto& mnemonic_token = token_buffer().at(mnemonic_pos.value());
        auto& mnemonic_name = std::get<std::string>(mnemonic_token.attribute());

//        // if an instruction definition exists for that mnemonic
//        if(spec::instructions.count(mnemonic_name)) {
//            auto& instruction_def = spec::instructions.at(mnemonic_name);
//
//            // correct operand format
//            if(instruction_def.m_operand_fmt == seq_kv.first) {
//
//            }
//        }
//
//        switch(ins.m_ins_format) {
//            case spec::R:
//
//            break;
//
//            case spec::I:
//            break;
//        }
//
//        binary |= (ins.m_lower_field);
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
