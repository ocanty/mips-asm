//
// Created by ocanty on 16/01/19.
//

#ifndef MIPS_ASM_LEXER_HPP
#define MIPS_ASM_LEXER_HPP

#include <cstdint>
#include <variant>
#include <sstream>

#include "../fsm/finite_state_machine.hpp"
#include "../fsm/transition.hpp"

namespace as {

class lexer {
public:
    lexer();
    virtual ~lexer() = default;

    enum class token_type : signed int {
        DIRECTIVE,


        INVALID_TOKEN = -1
    };

    class token {
    public:
        /**
         * Create a token
         * @param type Token type
         * @param attr Custom attribute, typically a symbol or a number tagged to that token (e.g. a label name)
         */
        explicit token(const token_type& type, const std::variant<std::string,std::uint32_t>& attr = 0) :
            m_type(type),
            m_attribute(attr)
        { }

        /**
         * @return Token type
         */
        token_type getType() const {
            return m_type;
        }

        /**
         * @return Variable associated with the token (either a number, or a symbol)
         */
        std::variant<std::string,std::uint32_t> getAttribute() const {
            return m_attribute;
        }

    private:
        token_type  m_type;

        /*
         * An optional attribute that the token may have
         * may be memory intensive, 
         * i.e symbols will be repeated rather than storing a reference to a dictionary
         * TODO: consider optimizing this
         */
        std::variant<std::string,std::uint32_t> m_attribute;
    };

    /**
     * Convert a string of MIPs assembly into tokens
     * @returns vector of token
     */
    std::optional<std::vector<token>> lex(const std::string& input);

private:
    /**
     * The possible states the lexer's FSM can be in
     */
    enum class states {
        // Base state, no input has occurred yet or we have returned from a token push
        BASE,

        // We have entered a directive, continue to eat characters till a space, push the token then go to BASE
        SEEK_DIRECTIVE,

        // Like SEEK_DIRECTIVE, but for labels
        SEEK_LABEL,


        INVALID_TOKEN = -1  // Unexpected token
    };

    /**
     * This is the class used for the inputs of the FSM
     */
    class lexer_data {
    public:
        /*
         * The current character that has been passed to the lexer
         * Note: m_char is the only variable the FSM depends on to determine state
         * The other variables are simple used for outputs
         *
         * Stored as an std::string to make passing to std::regex easier
         * */
        std::string m_char;

        /* The output tokens */
        std::vector<token> m_tokens;

        /* Character buffer */
        std::stringstream m_buffer;

        void push_token(const token_type& type, const std::variant<std::string,std::uint32_t>& attr = 0) {
            m_tokens.emplace_back(token(type, attr));
        }

        std::string get_buffer() {
            return m_buffer.str();
        }

        void clear_buffer() {
            m_buffer.clear(); // clear string
            m_buffer.str(""); // resets flags
        }
    };

    /*
     * Loads transitions into the FSM
     */
    void setup_fsm();

    finite_state_machine<states, lexer_data> m_fsm;
};

}
#endif //MIPS_ASM_LEXER_HPP
