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
    token(const token_type &type, const std::size_t& line, const std::variant <std::string, std::int32_t> &attr = 0) :
            m_type(type),
            m_attribute(attr),
            m_line(line)
    {

    }

    /**
     * @return Token type
     */
    const token_type& type() const {
        return m_type;
    }

    /**
     * @return Variable associated with the token (either a number, or a symbol)
     */
    const std::variant <std::string, std::int32_t>& attribute() const {
        return m_attribute;
    }

    const std::string& name() const {
        return token_type_name.at(m_type);
    }

    const std::size_t& line() const {
        return m_line;
    }
private:
    token_type m_type;

    // line where the token was found
    std::size_t m_line;

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
