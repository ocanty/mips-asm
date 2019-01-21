//
// Created by ocanty on 20/01/19.
//

#ifndef MIPS_ASM_TOKEN_TYPE_HPP
#define MIPS_ASM_TOKEN_TYPE_HPP

namespace as
{

enum class token_type : signed int {
    DIRECTIVE,
    MNEMONIC,
    LABEL,
    LABEL_DEFINITION,
    REGISTER,

    LITERAL,

    // The literal in IMM(register)
    LITERAL_IMM,

    // Register
    IMM_REGISTER,


    INVALID_TOKEN = -1
};

const std::unordered_map<token_type, std::string> token_type_name = {
    { token_type::DIRECTIVE, "Directive" },
    { token_type::MNEMONIC, "Mnemonic" },
    { token_type::LABEL, "Label" },
    { token_type::LABEL_DEFINITION, "Label:" },
    { token_type::REGISTER, "reg" },
    { token_type::LITERAL, "literal" },
    { token_type::LITERAL_IMM, "literal_imm" },
    { token_type::IMM_REGISTER, "(reg)" },
    { token_type::INVALID_TOKEN, "invalid" }

};

}


#endif //MIPS_ASM_TOKEN_TYPE_HPP
