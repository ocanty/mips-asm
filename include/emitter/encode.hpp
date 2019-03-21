//
// Created by ocanty on 21/03/19.
//

#ifndef MIPS_ASM_ENCODE_HPP
#define MIPS_ASM_ENCODE_HPP

#include <optional>
#include <vector>
#include "lexer/token.hpp"
#include "emitter/op_sequences.hpp"

namespace as {

/**
 * Encode an instruction using a token buffer, and a set of labels
 * @param tokens
 * @param labels
 * @return Optional encoded MIPs instruction if encoding worked
 *         Messages & errors are passed in the stringstream
 */
std::optional<std::uint32_t>
encode_instruction(const std::vector<token> &tokens,
                   const std::unordered_map<std::string, std::uint32_t> &labels = {});

}

#endif //MIPS_ASM_ENCODE_HPP
