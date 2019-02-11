//
// Created by ocanty on 20/01/19.
//

#ifndef MIPS_ASM_INSTRUCTION_HPP
#define MIPS_ASM_INSTRUCTION_HPP

#include <cstddef>
#include <list>
#include "../lexer/token_type.hpp"

namespace as::spec {

/**
 * The type of encoding an instruction uses
 */
enum instruction_def_format {
    R,
    I,
    J
};

/**
 * The types of an operand in an instruction
 */
enum operand_def_format {

    // Tri-reg
    RD_RS_RT,          // (R) - rd, rs, rt
    RD_RT_RS,          // (R) - rd, rt, rs

    // Duo-reg
    RS_RT,             // (R) - rs, rt
    RD_RS,             // (R) - rd, rs

    // Single-reg
    RS,                // (R) - rs
    RD,                // (R) - rd

    // Duo-reg + literal
    RS_RT_OFFSET,      // (I) - rs, rt, offset
    RT_RS_IMM,         // (I) - rs, rt, immediate
    RD_RT_SA,          // (R) - rd, rt, shift

    RS_OFFSET,         // (I) - rs, offset
    RT_IMM,            // (I) - rt, immediate

    RS_RT_OFFSET_BASE, // (I) - rs, rt, offset(base)
    RT_OFFSET_BASE,    // (I) - rt, offset(base)

    NO_OPERAND_W_CODE_20, // see syscall, break
    NO_OPERAND_W_1_BIT,   // see rfe, tlbp, tlbr, tlbwi, tlbwr

    TARGET             // (J) - target
};

/**
 * Defines an instruction
 */
class instruction_def {

public:
    /**
     * Define an instruction by it's encoding, operands & upper and lower fields
     * @param idf           Instruction encoding type
     * @param odf           Operand format type
     * @param upper_field   Instruction upper field, bits 31 to 26
     *                          for I format this is opcode,
     *                          for R format this is SPECIAL / see R3000 spec for clarification
     * @param lower_field   Instruction lower field, bits 0 to 6
     *                          for R format, the opcode is stored here,
     *                          more commonly known as the funct value/function type for ALU
     */
    instruction_def(const instruction_def_format& idf,
            const operand_def_format& odf,
            const std::uint8_t& upper_field,
            const std::uint8_t& lower_field);

    /**
     * Define an instruction by it's encoding, operands & upper field
     * @param idf           Instruction encoding type
     * @param odf           Operand format type
     * @param upper_field   Instruction upper field, bits 31 to 26
     *                          for I format this is opcode,
     *                          for R format this is SPECIAL / see R3000 spec for clarification
     */
    instruction_def(const instruction_def_format& idf,
            const operand_def_format& odf,
            const std::uint8_t& upper_field);

    /**
     * Get instruction encoding format
     * @return instruction format
     */
    const instruction_def_format&   instruction_format();

    /**
     * Get operand format
     * @return operand format
     */
    const operand_def_format&       operand_format();

private:
    instruction_def_format  m_ins_format;
    operand_def_format      m_operand_fmt;

    std::uint8_t m_upper_field;
    std::uint8_t m_lower_field;
};

/**
 * Static container class for all possible instruction definitions
 */
class instructions {
public:
    /**
     * Returns true if mnemonic is a valid instruction
     * @param mnemonic
     * @return True if mnemonic exists, else false
     */
    static bool exists(const std::string& mnemonic) {
        return instruction_defs.count(mnemonic) > 0;
    }

    /**
     * Get instruction definition for instruction of mnemonic
     * @param mnemonic
     * @return Optional of instruction definition
     */
    static std::optional<instruction_def> get(const std::string& mnemonic) {
        if(exists(mnemonic)) {
            return instruction_defs.at(mnemonic);
        }
    }
private:
    static const std::unordered_map<std::string, instruction_def> instruction_defs;
};

}

#endif //MIPS_ASM_INSTRUCTION_HPP
