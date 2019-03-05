//
// Created by ocanty on 09/02/19.
//

#include "spec/instruction_defs.hpp"

namespace as::spec {

instruction_def::instruction_def(const instruction_def_format& idf,
    const operand_def_format& odf,
    const std::uint8_t& upper_field,
    const std::uint8_t& lower_field) :
    m_ins_format(idf),
    m_operand_fmt(odf),
    m_upper_field(upper_field),
    m_lower_field(lower_field) {

};

instruction_def::instruction_def(const instruction_def_format& idf,
    const operand_def_format& odf,
    const std::uint8_t& upper_field) :
    m_ins_format(idf),
    m_operand_fmt(odf),
    m_upper_field(upper_field) {

};

const instruction_def_format& instruction_def::instruction_format() {
    return m_ins_format;
}

const operand_def_format & instruction_def::operand_format() {
    return m_operand_fmt;
}

const std::unordered_map<std::string, instruction_def> instructions::instruction_defs = {
    // Format Operand-format upper_field, lower_field
    {"add",   {R, RD_RS_RT,       0x00, 0x20}},
    {"addi",  {I, RT_RS_IMM,      0x08}},
    {"addiu", {I, RT_RS_IMM,      0x09}},
    {"addu",  {R, RD_RS_RT,       0x00, 0x21}},
    {"and",   {R, RD_RS_RT,       0x00, 0x24}},
    {"andi",  {I, RT_RS_IMM,      0x0C, 0X24}},
    {"beq",   {I, RS_RT_OFFSET,   0x04}},
    // missing: BGEZ
    // missing: BGEZAL
    {"bgtz",  {I, RS_OFFSET,      0x07}},
    // missing: BGTZ
    {"blez",  {I, RS_OFFSET,      0x06}},
    // missing: BLTZ
    // missing: BLTZAL
    {"bne",   {I, RS_RT_OFFSET,   0x05}},
    {"div",   {R, RS_RT,          0x00, 0x1A}},
    {"divu",  {R, RS_RT,          0x00, 0x1B}},
    {"j",     {J, TARGET,         0x02}},
    {"jal",   {J, TARGET,         0x03}},
    // missing: JALR
    {"jr",    {R, RS,             0x00, 0x08}},
    {"lb",    {I, RT_OFFSET_BASE, 0x20}},
    {"lbu",   {I, RT_OFFSET_BASE, 0x24}},
    {"lh",    {I, RT_OFFSET_BASE, 0b10001}},
    {"lhu",   {I, RT_OFFSET_BASE, 0x25}},
    {"lui",   {I, RT_IMM,         0x0F}},
    {"lw",    {I, RT_OFFSET_BASE, 0x23}},
    {"lwl",   {I, RT_OFFSET_BASE, 0b100010}},
    {"lwr",   {I, RT_OFFSET_BASE, 0b100110}},

    {"mfhi",  {R, RD,             0x00, 0x10}},
    {"mthi",  {R, RS,             0x00, 0x11}},
    {"mflo",  {R, RD,             0x00, 0x12}},
    {"mtlo",  {R, RS,             0x00, 0x13}},
    // { "mfc0",  { R,0x10}},
    {"mult",  {R, RS_RT,          0x00, 0x18}},
    {"multu", {R, RS_RT,          0x00, 0x19}},
    // { "nor",   { R,RD_RS_RT,0x00,0x27}},
    // { "xor",   { R,0x00,0x26}},
    {"or",    {R, RD_RS_RT,       0x00, 0x25}},
    {"ori",   {I, RT_RS_IMM,      0x0D}},
    {"sb",    {I, RT_OFFSET_BASE, 0b101000}},
    {"sh",    {I, RT_OFFSET_BASE, 0x29}},
    {"sll",   {R, RD_RT_SA,       0x00, 0x00}},
    {"sllv",  {R, RD_RT_RS,       0x00, 0b000100}},
    {"slt",   {R, RD_RT_SA,       0x00, 0x2A}},
    {"slti",  {I, RT_RS_IMM,      0x0A}},
    {"sltiu", {I, RT_RS_IMM,      0x0B}},
    {"sltu",  {R, RD_RS_RT,       0x00, 0x2B}},

    {"sra",   {R, RD_RT_SA,       0x00, 0x03}},
    {"srav",  {R, RD_RT_RS,       0x00, 0b000111}},
    {"srl",   {R, RD_RT_SA,       0x00, 0b000010}},
    {"srlv",  {R, RD_RT_RS,       0x00, 0b000110}},
    {"sub",   {R, RD_RS_RT,       0x00, 0x22}},
    {"subu",  {R, RD_RS_RT,       0x00, 0x23}},
    {"sw",    {I, RT_OFFSET_BASE, 0b101011}},
    {"swl",   {I, RT_OFFSET_BASE, 0b101010}},
    {"swl",   {I, RT_OFFSET_BASE, 0b101010}}
};

}