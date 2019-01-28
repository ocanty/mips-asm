//
// Created by ocanty on 20/01/19.
//

#ifndef MIPS_ASM_INSTRUCTION_HPP
#define MIPS_ASM_INSTRUCTION_HPP

#include <cstddef>
#include <list>
#include "../lexer/token_type.hpp"

namespace as::spec {

enum instruction_format {
    R,
    I,
    J
};

enum operand_format {

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


struct instruction {
    instruction_format m_ins_format;

    // Bits 31 to 26
    // For I (immediate format), the opcode is usually stored here
    //
    // For R (register format), it is known as the SPECIAL value in the spec
    // Despite this, many books still refer to this as the opcode
    std::uint8_t m_upper_field;

    // Bits 0 to 6
    // The opcode in the R (register format) (as per the spec) is stored here,
    // This is known more commonly as funct value
    std::uint8_t m_lower_field;

    // see:
    // https://www.cs.cornell.edu/courses/cs3410/2008fa/MIPS_Vol2.pdf
    // https://en.wikibooks.org/wiki/MIPS_Assembly/Instruction_Formats
    // std::vector<asm_format> m_asm_format;

    operand_format      m_operand_fmt;

};

}

#endif //MIPS_ASM_INSTRUCTION_HPP
