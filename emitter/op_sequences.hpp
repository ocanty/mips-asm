//
// Created by ocanty on 28/01/19.
//

#ifndef MIPS_ASM_TOKEN_OPERANDS_MAP_HPP
#define MIPS_ASM_TOKEN_OPERANDS_MAP_HPP

#include <optional>
#include <vector>
#include <map>
#include "../spec/instruction_defs.hpp"
#include "../lexer/token_type.hpp"
#include "../lexer/token.hpp"

namespace as {

    using namespace spec;

/**
 * An operation sequence stores a sequence of token types
 * that represent a operation
 *
 * It also contains a mapping of attribute names, i.e "rd", "shamt", "rs",
 * mapping name to the index of the token the attribute can be found
 */
class op_sequence {
public:

    /**
     * The operands in the sequence are defined by their operand format,
     * We need to specify which tokens map to which operand,
     * for each possible operand format that this sequence should support
     *
     * i.e token 3's attribute might be the register number for the target register (rt)
     * so
     *
     * This sequence of token types might be
     * e.g. MNEMONIC, REGISTER, COMMA, REGISTER, COMMA, REGISTER
     *
     * operand_locations[spec::RD_RT_RS] = { {"rt",3}, ... }
     *                    ^                     ^- Token mapping
     *                    \- i.e. This sequence maps to this operand format
     */
    using operand_locations =
        std::unordered_map<spec::operand_def_format, std::unordered_map<std::string, std::size_t>>;

    op_sequence(
        const std::vector<token_type>& token_type,
        const operand_locations& locations
    );

    explicit op_sequence(const std::vector<token_type>& types);

    /**
     * Get token type array
     * @return
     */
    const std::vector<token_type>& token_types() const;

    /**
     * Get the position of an operand in an operand format that this class supports
     * @param fmt Operand format
     * @param name Operand name
     */
     std::optional<std::size_t> operand_position(
        const as::operand_def_format &fmt,
        const std::string &name
     ) const;

private:
    std::vector<token_type> m_token_types;
    operand_locations m_operand_locations = { };
};

/**
 * Static container class for operation sequences
 */
class op_sequences {
public:
    /**
     * Get a reference to all possible op sequences
     * @return
     */
    static const std::vector<op_sequence>& all() {
        return sequences;
    }
private:
    static const std::vector<op_sequence> sequences;
};

}

#endif //MIPS_ASM_TOKEN_OPERANDS_MAP_HPP
