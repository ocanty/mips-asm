//
// Created by ocanty on 20/01/19.
//

#ifndef MIPS_ASM_TOKEN_TYPE_HPP
#define MIPS_ASM_TOKEN_TYPE_HPP

#include <string>
#include <unordered_map>

namespace as
{

enum class token_type : signed int {
    // A directive .text
    DIRECTIVE,

    // An instruction mnemonic, i.e addi
    MNEMONIC,

    // A label, i.e. start
    LABEL,

    // A label definition, i.e _start:
    LABEL_DEFINITION,

    // A register, $t0
    REGISTER,

    // A string - e.g. "hello"
    LITERAL_STRING,

    // A character e.g. 'h'
    LITERAL_CHAR,

    // A literal number 0x00, 247, -128, etc...
    LITERAL_NUMBER,

    // The literal (number) in offset(register)
    OFFSET,

    // Register
    BASE_REGISTER,

    // ","
    COMMA,

    // This is a token as assembly statements are broken by newlines
    NEW_LINE,

    // Unknown token
    INVALID_TOKEN = -1
};

/* String map for enum */
const std::unordered_map<token_type, std::string> token_type_name = {
    { token_type::DIRECTIVE, "DIRECTIVE" },
    { token_type::MNEMONIC, "MNEMONIC" },
    { token_type::LABEL, "LABEL" },
    { token_type::LABEL_DEFINITION, "LABEL_DEF" },
    { token_type::REGISTER, "REGISTER" },
    { token_type::LITERAL_STRING, "LITERAL_STRING" },
    { token_type::LITERAL_CHAR, "LITERAL_CHAR" },
    { token_type::LITERAL_NUMBER, "LITERAL_NUMBER" },
    { token_type::OFFSET, "OFFSET" },
    { token_type::BASE_REGISTER, "(BASE_REGISTER)" },
    { token_type::INVALID_TOKEN, "INVALID_TOKEN" },
    { token_type::COMMA, "," },
    { token_type::NEW_LINE, "NEW_LINE"}
};

const std::string& get_name_for_token(const token_type& tk);

}


#endif //MIPS_ASM_TOKEN_TYPE_HPP
