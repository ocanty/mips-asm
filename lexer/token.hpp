//
// Created by ocanty on 20/01/19.
//

#ifndef MIPS_ASM_TOKEN_HPP
#define MIPS_ASM_TOKEN_HPP

#include <variant>
#include "token_type.hpp"

namespace as {

class token {
public:
    /**
     * Create a token
     * @param type Token type
     * @param attr Custom attribute, typically a symbol or a number tagged to that token (e.g. a label name)
     */
    explicit token(const token_type &type, const std::variant <std::string, std::int32_t> &attr = 0) :
            m_type(type),
            m_attribute(attr) {}

    /**
     * @return Token type
     */
    token_type getType() const {
        return m_type;
    }

    /**
     * @return Variable associated with the token (either a number, or a symbol)
     */
    std::variant <std::string, std::int32_t> getAttribute() const {
        return m_attribute;
    }


    std::string getName() const {
        return token_type_name.at(m_type);
    }
private:
    token_type m_type;

    /*
     * An optional attribute that the token may have
     * may be memory intensive,
     * i.e symbols will be repeated rather than storing a reference to a dictionary
     * TODO: consider optimizing this
     */
    std::variant <std::string, std::int32_t> m_attribute;
};

}
#endif //MIPS_ASM_TOKEN_HPP
