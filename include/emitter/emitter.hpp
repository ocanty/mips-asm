//
// Created by ocanty on 28/01/19.
//

#ifndef MIPS_ASM_emitter_HPP
#define MIPS_ASM_emitter_HPP

#include <vector>
#include <sstream>
#include <map>
#include "../lexer/token.hpp"
#include "op_sequences.hpp"
#include "../spec/instruction_defs.hpp"


namespace as {
/**
 *
 * @param tokens
 * @return
 */
std::optional<std::vector<std::uint8_t>>
emit(const std::vector<token>& tokens);

}

#endif //MIPS_ASM_emitter_HPP
