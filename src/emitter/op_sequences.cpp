//
// Created by ocanty on 06/02/19.
//

#include "emitter/op_sequences.hpp"
#include "emitter/emitter_context.hpp"
#include <map>
#include <unordered_map>

namespace as {


op_sequence::op_sequence(
    const std::vector<as::token_type> &token_types,
    const as::op_sequence::operand_locations &locations) :
    m_token_types(token_types),
    m_operand_locations(locations) {

}

op_sequence::op_sequence(const std::vector<token_type> &types) :
    m_token_types(types) {

}


const std::vector<token_type>& op_sequence::token_types() const {
    return m_token_types;
}

bool op_sequence::supports_operand_format(const spec::operand_def_format& fmt) const {
    return (m_operand_locations.count(fmt) > 0);
}


std::optional<std::size_t> op_sequence::operand_position(
        const as::operand_def_format &fmt,
        const std::string &name) const {

    // if this sequence has the operands required for the operand format
    if(supports_operand_format(fmt)) {

        // if the operand format has an operand that matches the one they want
        if(m_operand_locations.at(fmt).count(name) > 0) {
            return m_operand_locations.at(fmt).at(name);
        }
    }

    return std::nullopt;
}

using t = token_type;
const std::vector<op_sequence> op_sequences::sequences = {
    {   // op $reg, $reg, $reg
        { t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::REGISTER },
        {
            { spec::RD_RS_RT, { {"rd", 1}, {"rs", 3}, {"rt", 5} } },
            { spec::RD_RT_RS, { {"rd", 1}, {"rs", 5}, {"rt", 3} } }
        }
    },

    {   // op $reg, $reg
        { t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER },
        {
            { spec::RD_RS, { {"rd", 1}, {"rs", 3} } },
            { spec::RS_RT, { {"rs", 1}, {"rt", 3} } }
        },
    },

    {   // op $reg
        { t::MNEMONIC, t::REGISTER },
        {
            { spec::RD, { {"rd", 1} } },
            { spec::RS, { {"rs", 1} } }
        },
    },

    {   // op $reg, $reg, offset
        //                ^-- a literal number
        { t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::LITERAL_NUMBER },
        {
            { spec::RS_RT_OFFSET, { {"rs", 1}, {"rt", 3}, {"imm", 5 } } },
            { spec::RD_RT_SA,     { {"rd", 1}, {"rt", 3}, {"shamt", 5 } } },
        }
    },

    {   // op $reg, offset($reg_base)
        {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::OFFSET, t::BASE_REGISTER },
        {
            {spec::RS_RT_OFFSET_BASE, { {"rs",1}, {"rt",3}, {"imm",5}, {"rd",6} } }
        }
    },

    {   // op $reg, offset
        //              ^-- a literal number
        {t::MNEMONIC, t::REGISTER, t::COMMA, t::LITERAL_NUMBER },
        {
            { spec::RS_OFFSET, { {"rs", 1}, {"imm", 3} } },
            { spec::RT_IMM, { {"rt", 1}, {"imm", 3} } }
        }
    },


    {
        {t::MNEMONIC, t::LITERAL_NUMBER },
        {
            {spec::TARGET, { {"imm", 1 } } }
        }
    },

    { // no operand
        {t::MNEMONIC }, { }
    },
};

}