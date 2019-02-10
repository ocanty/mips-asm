//
// Created by ocanty on 06/02/19.
//

#include "op_sequences.hpp"

namespace as {

op_sequence::op_sequence(const std::vector<token_type>& types) :
    m_token_types(types) {

};

op_sequence::op_sequence(const std::vector<token_type>& types,
    const std::unordered_map<std::string, std::size_t>& positions) :
    m_token_types(types),
    m_positions(positions) {

    // check if mnemonic hasn't been overriden
    if(!m_positions.count("mnemonic")) {

        // first token is usually the mnemonic
        // if it's not they'd have set it
        m_positions.insert({"mnemonic", 0});
    }
}

const std::vector<token_type>& op_sequence::token_types() const {
    return m_token_types;
}

std::optional<std::size_t> op_sequence::position_of(const std::string& attr_name) const {
     if(m_positions.count(attr_name)) {
         return m_positions.at(attr_name);
     }

    return std::nullopt;
}

}