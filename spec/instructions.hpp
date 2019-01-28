
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
//    { "add",     { R, 0x00, 0x20 } },
//    { "addi",    { I, 0x08       } },
//    { "addiu",   { I, 0x09       } },
//    { "addu",    { R, 0x00, 0x21 } },
//    { "and",     { R, 0x00, 0x24 } },
//    { "andi",    { I, 0x0C       } },
//    // // idiom Format: B offset -> BEQ r0, r0, offset.

//    { "beq",     { I, 0x4        } },
//    { "blez",    { I, 0x6        } },
//    { "bne",     { I, }}
    {"add", {R,0x00,0x20, RD_RS_RT}},
//    {"addi", {I,0x08,}},
//    {"addiu", {I,0x09}},
//    {"addu", {R,0x00,0x21}},
//    {"and", {R,0x00,0x24}},
//    {"andi", {I,0x0C,}},
//    {"beq", {I,0x04}},
//    {"blez", {I,0x06}},
//    {"bne", {I,0x05}},
//    {"bgtz", {I,0x07}},
//    {"div", {R,0x00,0x1A}},
//    {"divu", {R,0x00,0x1B}},
//    {"j", {J,0x02}},
//    {"jal", {J,0x03}},
//    {"jr", {R,0x00,0x08}},
//    {"lb", {I,0x20}},
//    {"lbu", {I,0x24}},
//    {"lhu", {I,0x25}},
//    {"lui", {I,0x0F}},
//    {"lw", {I,0x23}},
//    {"mfhi", {R,0x00,0x10}},
//    {"mthi", {R,0x00,0x11}},
//    {"mflo", {R,0x00,0x12}},
//    {"mtlo", {R,0x00,0x13}},
//    {"mfc0", {R,0x10}},
//    {"mult", {R,0x00,0x18}},
//    {"multu", {R,0x00,0x19}},
//    {"nor", {R,0x00,0x27}},
//    {"xor", {R,0x00,0x26}},
//    {"or", {R,0x00,0x25}},
//    {"ori", {I,0x0D}},
//    {"sb", {I,0x28}},
//    {"sh", {I,0x29}},
//    {"slt", {R,0x00,0x2A}},
//    {"slti", {I,0x0A}},
//    {"sltiu", {I,0x0B}},
//    {"sltu", {R,0x00,0x2B}},
//    {"sll", {R,0x00,0x00}},
//    {"srl", {R,0x00,0x02}},
//    {"sra", {R,0x00,0x03}},
//    {"sub", {R,0x00,0x22}},
//    {"subu", {R,0x00,0x23}},
//    {"sw", {I,0x2B}}

};

};

#endif