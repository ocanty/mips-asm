//
// Created by ocanty on 20/01/19.
//

#ifndef MIPS_ASM_INSTRUCTION_HPP
#define MIPS_ASM_INSTRUCTION_HPP

#include <cstddef>

namespace as::spec {

enum instruction_format {
    R,
    I,
    J
};

struct instruction {
    instruction_format m_format;

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
};

}

#endif //MIPS_ASM_INSTRUCTION_HPP
