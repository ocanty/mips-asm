//
// Created by ocanty on 28/01/19.
//

#ifndef MIPS_ASM_TOKEN_OPERANDS_MAP_HPP
#define MIPS_ASM_TOKEN_OPERANDS_MAP_HPP

#include <optional>
#include <vector>
#include <map>
#include "../spec/instruction_defs.hpp"
#include "../lexer/token_type.hpp"
#include "../lexer/token.hpp"

namespace as {

    using namespace spec;

/**
 * An operation sequence stores a sequence of token types
 * that represent a operation
 *
 * It also contains a mapping of attribute names, i.e "rd", "shamt", "rs",
 * mapping name to the index of the token the attribute can be found
 */
class op_sequence {
public:
    op_sequence(const std::vector<token_type>& token_types);

    op_sequence(const std::vector<token_type>& token_types,
        const std::unordered_map<std::string, std::size_t>& attribute_positions);

    /**
     * Get token type array
     * @return
     */
    const std::vector<token_type>& token_types() const;

    /**
     * Get the position of an attribute
     * @param name Attribute name
     */
     std::optional<std::size_t> position_of(const std::string& name) const;

private:
    std::vector<token_type> m_token_types;
    std::unordered_map<std::string, std::size_t> m_positions =
            { {"mnemonic",0} };
};

using t = token_type;

/* these represent token sequences of any possible operands for a given operand format */

// 3 register operations
const std::vector<token_type> op_tri_reg =
    {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::REGISTER, t::NEW_LINE};

// 2 register operations
const std::vector<token_type> op_duo_reg =
    {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::NEW_LINE};

// 2 register + number operations, i.e. rs, rt, offset
const std::vector<token_type> op_duo_reg_number =
    {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::LITERAL_NUMBER, t::NEW_LINE};

// 1 register + number
const std::vector<token_type> op_single_reg_number =
    {t::MNEMONIC, t::REGISTER, t::COMMA, t::LITERAL_NUMBER, t::NEW_LINE};

// 2 register + offset(base), i.e. rs, rt offset(base) -> $t0, $t1, -4($02)
const std::vector<token_type> op_duo_reg_offset_base =
    {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::OFFSET, t::BASE_REGISTER, t::NEW_LINE};

// 1 register + offset(base)
const std::vector<token_type> op_single_reg_offset_base =
    {t::MNEMONIC, t::REGISTER,  t::COMMA, t::OFFSET, t::BASE_REGISTER, t::NEW_LINE};

// No operand
const std::vector<token_type> op_no_operand =
    {t::MNEMONIC,  t::NEW_LINE};

// addr operand, i.e j 0x5000
const std::vector<token_type> op_target_addr =
    {t::MNEMONIC, t::LITERAL_NUMBER};

// label operand
const std::vector<token_type> op_target_label =
    {t::MNEMONIC, t::LABEL};

/* This table links operand format and sequence required */
const std::map<spec::operand_def_format, op_sequence> op_sequences = {
    {
        spec::RD_RS_RT,
        { op_tri_reg, { {"rd", 1}, {"rs", 3}, {"rt", 5} } }
    },

    {
        spec::RD_RT_RS,
        { op_tri_reg, { {"rd", 1}, {"rs", 5}, {"rt", 3} } }
    },

    {
        spec::RS_RT,
        { op_duo_reg, { {"rs", 1}, {"rt", 3} } }
    },

    {
        spec::RD_RS,
        { op_duo_reg, { {"rd", 1}, {"rs", 3} } }
    },

    {
        spec::RS,
        { op_duo_reg, { {"rs", 1} } }
    },

    {
        spec::RD,
        { op_duo_reg, { {"rd", 1} } }
    },

    {
        spec::RS_RT_OFFSET,
        { op_duo_reg, { {"rd", 1} } }
    },

    {
        spec::RT_RS_IMM,
        { op_duo_reg_number, { {"rs",1}, {"rt",3}, {"imm",5} } }
    },

    {
        spec::RD_RT_SA,
        { op_duo_reg_number, { {"rd",1}, {"rt",3}, {"shamt",5} } }
    },

    {
        spec::RS_OFFSET,
        { op_single_reg_number, { {"rs",1}, {"imm",3} } }
    },

    {
        spec::RT_IMM,
        { op_single_reg_number, { {"rt",1}, {"imm",3} } }
    },

    {
        spec::RS_RT_OFFSET_BASE,
        { op_duo_reg_offset_base, { {"rd",6}, {"rs",1}, {"rt",3}, {"imm",6} } }
    },

    {
        spec::RT_OFFSET_BASE,
        { op_single_reg_offset_base, { {"rt", 1} } }
    },

//    {
//        spec::TARGET,
//        { op_target_addr, { .addr = 1 } }
//    },
//
//    {
//        spec::TARGET,
//        { op_target_label, { .addr = 1, .label_on = 1 } }
//    }
};

}

#endif //MIPS_ASM_TOKEN_OPERANDS_MAP_HPP
