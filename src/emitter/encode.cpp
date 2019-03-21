//
// Created by ocanty on 21/03/19.
//

#include <numeric>
#include <algorithm>
#include <string>
#include <sstream>
#include <utility>
#include <optional>
#include "emitter/encode.hpp"

namespace as {

std::optional<std::uint32_t>
encode_instruction(const std::vector<token> &tokens,
                   const std::unordered_map<std::string, std::uint32_t>& labels) {

    std::stringstream log;

    // check if the sequence of tokens match an operation

    // first get token_types for all the tokens passed
    std::vector<token_type> token_types = std::accumulate(
        tokens.begin(),
        tokens.end(),
        std::vector<token_type>{},
        [](std::vector<token_type>& token_types, const token& token) {
            token_types.emplace_back(token.type());
            return token_types;
        });

    bool valid_sequence = false;
    op_sequence sequence;

    // check against each possible operation sequence that our assembler supports
    for(auto& seq : op_sequences::all()) {
        if (token_types == seq.token_types()) {
            sequence = seq;
            valid_sequence = true;
            break;
        }
    }

    if(!valid_sequence) {
        log << "Unknown sequence of tokens ";

        if(tokens.size() > 0) {
            log << "near line "
                     << tokens.at(0).line()
                     << " ";
        }

        log << std::endl;
        return std::nullopt;
    }

    if(tokens.empty()) {
        log << "Tried to encode an instruction with an empty token buffer. "
            << "This should never happen!"
            << std::endl;
        return std::nullopt;
    }

    auto& mnemonic_token = tokens.at(0);

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
                auto label_position  = sequence.operand_position(operand_fmt, "label");

                auto rs = (rs_position.has_value() ?
                    std::get<std::int32_t>(tokens.at(rs_position.value()).attribute()) : 0);

                auto rt = (rt_position.has_value() ?
                    std::get<std::int32_t>(tokens.at(rt_position.value()).attribute()) : 0);

                auto rd = (rd_position.has_value() ?
                    std::get<std::int32_t>(tokens.at(rd_position.value()).attribute()) : 0);

                auto imm = (imm_position.has_value() ?
                    std::get<std::int32_t>(tokens.at(imm_position.value()).attribute()) : 0);

                auto shamt = (shamt_position.has_value() ?
                    std::get<std::int32_t>(tokens.at(shamt_position.value()).attribute()) : 0);

                // labels
                std::string label = "";

                if(label_position.has_value()) {
                    label = std::get<std::string>(tokens.at(label_position.value()).attribute());

                    if(label.size() && labels.count(label)) {
                        imm = labels.at(label);
                    }
                }

                if(rs > 31 || rs < 0 || rt > 31 || rt < 0 || rd > 31 || rd < 0) {
                    log << "Invalid register ranges (e.g. $x, where 0 <= x <= 31, in instruction near line "
                        << mnemonic_token.line()
                        << std::endl;

                    return std::nullopt;
                }

                if(shamt > 31 || shamt < 0) {
                    log << "Invalid shift amounts in instruction near line "
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
                        std::cout << std::bitset<32>(ins) << std::endl;
                        ins |= ((rs     & 0b11111) << 21);
                        std::cout << std::bitset<32>(ins) << std::endl;
                        ins |= ((rt     & 0b11111) << 16);
                        std::cout << std::bitset<32>(ins) << std::endl;
                        ins |= ((imm & 0xFFFF));
                        std::cout << std::bitset<32>(ins) << std::endl;

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
                log << "Invalid operands near "
                    << mnemonic_name
                    << " near line "
                    << tokens.at(0).line()
                    << std::endl;
            }
        }
        else {
            log << "Invalid mnemonic "
                << mnemonic_name
                << " near line "
                << tokens.at(0).line()
                << std::endl;
        }
    }
    catch(std::bad_variant_access& e) {
        log  << "Bad attribute "
             << " near line "
             << tokens.at(0).line()
             << std::endl;
    }

    return std::nullopt;
}

}