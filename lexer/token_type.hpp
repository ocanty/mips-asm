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

    INVALID_TOKEN = -1
};

}


#endif //MIPS_ASM_TOKEN_TYPE_HPP
