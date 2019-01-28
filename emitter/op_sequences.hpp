//
// Created by ocanty on 28/01/19.
//

#ifndef MIPS_ASM_TOKEN_OPERANDS_MAP_HPP
#define MIPS_ASM_TOKEN_OPERANDS_MAP_HPP

#include <optional>
#include <vector>
#include <map>
#include "../spec/instruction.hpp"
#include "../lexer/token_type.hpp"

namespace as {
    using namespace spec;

    /** struct which specifies where an operand is in relation to a token sequence */
    struct operand_position {
        std::optional<std::size_t> op    = 0;               // mnemonic is typically at start
        std::optional<std::size_t> rd    = std::nullopt;
        std::optional<std::size_t> rs    = std::nullopt;
        std::optional<std::size_t> rt    = std::nullopt;
        std::optional<std::size_t> imm   = std::nullopt;
        std::optional<std::size_t> addr  = std::nullopt;
        std::optional<std::size_t> shamt = std::nullopt;

        std::optional<std::size_t> label_on = std::nullopt;

    };

    struct op_sequence {
        std::vector<token_type> tokens;
        operand_position operand_positions;
    };

    using t = token_type;

    const std::vector<token_type> op_tri_reg =
        {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::REGISTER, t::NEW_LINE};

    const std::vector<token_type> op_duo_reg =
        {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::NEW_LINE};

    const std::vector<token_type> op_duo_reg_number =
        {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::LITERAL_NUMBER, t::NEW_LINE};

    const std::vector<token_type> op_single_reg_number =
        {t::MNEMONIC, t::REGISTER, t::COMMA, t::LITERAL_NUMBER, t::NEW_LINE};

    const std::vector<token_type> op_duo_reg_offset_base =
        {t::MNEMONIC, t::REGISTER, t::COMMA, t::REGISTER, t::COMMA, t::OFFSET, t::BASE_REGISTER, t::NEW_LINE};

    const std::vector<token_type> op_single_reg_offset_base =
        {t::MNEMONIC, t::REGISTER,  t::COMMA, t::OFFSET, t::BASE_REGISTER, t::NEW_LINE};

    const std::vector<token_type> op_no_operand =
        {t::MNEMONIC,  t::NEW_LINE};

    const std::vector<token_type> op_target_addr =
        {t::MNEMONIC, t::LITERAL_NUMBER};

    const std::vector<token_type> op_target_label =
        {t::MNEMONIC, t::LABEL};

    const std::map<spec::operand_format, op_sequence> op_sequences = {
        {
            spec::RD_RS_RT,
            { op_tri_reg, { .rd = 1, .rs = 3, .rt = 5 } }
        },

        {
            spec::RD_RT_RS,
            { op_tri_reg, { .rd = 1, .rs = 5, .rt = 3 } }
        },

        {
            spec::RS_RT,
            { op_duo_reg, { .rs = 1, .rt = 3 } }
        },

        {
            spec::RD_RS,
            { op_duo_reg, { .rd = 1, .rs = 3 } }
        },

        {
            spec::RS,
            { op_duo_reg, { .rs = 1 } }
        },

        {
            spec::RD,
            { op_duo_reg, { .rd = 1 } }
        },

        {
            spec::RS_RT_OFFSET,
            { op_duo_reg, { .rd = 1,  } }
        },

        {
            spec::RT_RS_IMM,
            { op_duo_reg_number, { .rs = 1, .rt = 3, .imm = 5 } }
        },

        {
            spec::RD_RT_SA,
            { op_duo_reg_number, { .rd = 1, .rt = 3, .shamt = 5 } }
        },

        {
            spec::RS_OFFSET,
            { op_single_reg_number, { .rs = 1, .imm = 3 } }
        },

        {
            spec::RT_IMM,
            { op_single_reg_number, { .rt = 1, .imm = 3 } }
        },

        {
            spec::RS_RT_OFFSET_BASE,
            { op_duo_reg_offset_base, { .rd = 6, .rs = 1, .rt = 3, .imm = 6 } }
        },

        {
            spec::RT_OFFSET_BASE,
            { op_single_reg_offset_base, { .rt = 1, } }
        },

        {
            spec::TARGET,
            { op_target_addr, { .addr = 1 } }
        },

        {
            spec::TARGET,
            { op_target_label, { .addr = 1, .label_on = 1 } }
        }
    };

}

#endif //MIPS_ASM_TOKEN_OPERANDS_MAP_HPP
