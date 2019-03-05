//
// Created by ocanty on 09/02/19.
//

#include "emitter/emitter_context.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>

namespace as {

emitter_context::emitter_context(const as::token &tk) :
    m_token(tk) {
}

const token& emitter_context::cur_token() const {
    return m_token;
}

void emitter_context::set_cur_token(const token& tk) {
    m_token = tk;
}

std::vector<token>& emitter_context::token_buffer() {
    return m_tokens_in;
}

std::vector<token_type> emitter_context::token_types_buffer() const {
    return std::accumulate(
            m_tokens_in.begin(),
            m_tokens_in.end(),
            std::vector<token_type>{},
            [](std::vector<token_type>& token_types, const token& token) {
                token_types.emplace_back(token.type());
                return token_types;
            });
}

std::uint32_t emitter_context::get_text_label(const std::string& label) {
    return m_text_labels.at(label);
}

void emitter_context::set_current_text_label(const std::string& label) {
    if(m_text_labels.count(label)) {
        std::cout << "warning: label redefinition, the new label will be used instead ("
                  << label
                  << ") "
                  << std::endl;
    }

    m_text_labels.at(label) = SECTION_TEXT_BASE + m_text.size();
}

std::optional<std::uint32_t> emitter_context::encode_instruction(const op_sequence& sequence) {

    if(token_buffer().empty()) {
        std::cout << "Tried to encode an instruction with an empty token buffer. "
                  << "This should never happen!"
                  << std::endl;
        return false;
    }

    auto& mnemonic_token = token_buffer().at(0);

    try {
        auto &mnemonic_name = std::get<std::string>(mnemonic_token.attribute());

        // get the instruction definition for this mnemonic
        auto instruction_def = spec::instructions::get(mnemonic_name);

        // if there was a definition
        if(instruction_def.has_value()) {

            auto& operand_fmt = instruction_def.value().operand_format();

            // check if the sequence supports the operand format of the instruction/mnemonic
            if(sequence.supports_operand_format(operand_fmt)) {

                // instruction & sequence are both valid
                // lets encode

                // get positions of every possible operand
                auto rs_position     = sequence.operand_position(operand_fmt, "rs");
                auto rd_position     = sequence.operand_position(operand_fmt, "rd");
                auto rt_position     = sequence.operand_position(operand_fmt, "rt");
                auto imm_position    = sequence.operand_position(operand_fmt, "imm");
                // auto offset_position = sequence.operand_position(operand_fmt, "off");
                auto shamt_position  = sequence.operand_position(operand_fmt, "shamt");

                auto rs = (rs_position.has_value() ?
                    std::get<std::int32_t>(token_buffer().at(rs_position.value()).attribute()) : 0);

                auto rt = (rt_position.has_value() ?
                    std::get<std::int32_t>(token_buffer().at(rt_position.value()).attribute()) : 0);

                auto rd = (rd_position.has_value() ?
                    std::get<std::int32_t>(token_buffer().at(rd_position.value()).attribute()) : 0);

                auto imm = (imm_position.has_value() ?
                    std::get<std::int32_t>(token_buffer().at(imm_position.value()).attribute()) : 0);

                auto shamt = (shamt_position.has_value() ?
                    std::get<std::int32_t>(token_buffer().at(shamt_position.value()).attribute()) : 0);

                if(rs > 31 || rs < 0 || rt > 31 || rt < 0 || rd > 31 || rd < 0) {
                    errors() << "Invalid register ranges (e.g. $x, where 0 <= x <= 31, in instruction near line "
                             << mnemonic_token.line()
                             << std::endl;

                    return std::nullopt;
                }

                if(shamt > 31 || shamt < 0) {
                    errors() << "Invalid shift amounts in instruction near line "
                             << mnemonic_token.line()
                             << std::endl;
                }

                // get base value of instruction
                // this encodes the upper and lower fields for us
                // i.e the SPECIAL value, and func value for ALU instructions
                std::uint32_t ins = instruction_def.value().encoded();

                switch(instruction_def.value().instruction_format()) {
                    case spec::R:
                        ins |= ((rs     & 0b11111) << 21);
                        ins |= ((rt     & 0b11111) << 16);
                        ins |= ((rd     & 0b11111) << 11);
                        ins |= ((shamt  & 0b11111) <<  6);
                        std::cout << std::bitset<32>(ins) << std::endl;
                        return ins;
                    break;

                    case spec::I:
                        ins |= ((rs     & 0b11111) << 21);
                        ins |= ((rt     & 0b11111) << 16);
                        ins |= ((imm & 16));

                        return ins;
                    break;

                    case spec::J:
                        ins |= ((imm & 0x0FFFFFFF) >> 2) & 0b00000011111111111111111111111111;
                        std::cout << std::bitset<32>(ins) << std::endl;
                        return ins;
                    break;
                }
            }
            else {
                errors() << "Invalid operands near "
                         << mnemonic_name
                         << " near line "
                         << token_buffer().at(0).line()
                         << std::endl;
            }
        }
        else {
            errors() << "Invalid mnemonic "
                     << mnemonic_name
                     << " near line "
                     << token_buffer().at(0).line()
                     << std::endl;
        }
    }
    catch(std::bad_variant_access& e) {
        errors()  << "Bad attribute "
                  << " near line "
                  << token_buffer().at(0).line()
                  << std::endl;
    }

    return false;
}

std::stringstream& emitter_context::errors() {
    return m_errors;
}

}