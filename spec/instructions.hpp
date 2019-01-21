
#ifndef MIPS_ASM_INSTRUCTIONS_HPP
#define MIPS_ASM_INSTRUCTIONS_HPP

#include <unordered_map>
#include <string>

#include "instruction.hpp"

namespace as::spec {

const std::unordered_map<std::string, instruction> instructions = {
    // Sources: https://www.cs.cornell.edu/courses/cs3410/2008fa/MIPS_Vol2.pdf
    //          https://s3-eu-west-1.amazonaws.com/downloads-mips/documents/MD00565-2B-MIPS32-QRC-01.01.pdf
    // We are not implementing floating points, or coprocessor instructions, yet.

    //        Format, upper_field, lower_field
    {"add",     { R, 0x00, 0x20 } },
    {"addi",    { I, 0x08       } },
    {"addiu",   { I, 0x09       } },
    {"addu",    { R, 0x00, 0x21 } },
    {"and",     { R, 0x00, 0x24 } },
    {"andi",    { I, 0x0C       } },
    // // idiom Format: B offset -> BEQ r0, r0, offset.

    {"beq",     { I, 0x4        } },


};

};

#endif