//
// Created by ocanty on 16/01/19.
//

#include <sstream>
#include <regex>
#include "lexer.hpp"
#include "../fsm/transition.hpp"

namespace as {

lexer::lexer() :
    m_fsm(states::BASE) {
    setup_fsm();
}

void lexer::setup_fsm() {
    // handler for invalid tokens
    const transition<states, lexer_data>::transition_callback_func on_invalid_token =
        [](lexer_data& lex) {
            lex.push_token(token_type::INVALID_TOKEN, lex.get_buffer());
            lex.clear_buffer();
    };

    // This function when called, returns a should_transition_func
    // that will return true when a char matches the supplied regex
    auto match_pattern = [](const std::string& regex) -> auto {
        return transition<states, lexer_data>::should_transition_func(
            [regex](lexer_data& lex) -> bool {
                return std::regex_match(lex.m_char, std::regex(regex));
            }
        );
    };

    // the above, just not'ed
    auto not_match_pattern = [](const std::string& regex) -> auto {
        return transition<states, lexer_data>::should_transition_func(
            [regex](lexer_data& lex) -> bool {
                return !std::regex_match(lex.m_char, std::regex(regex));
            }
        );
    };

    // Begin directive - '.data'
    //                        ^
    m_fsm.add_transition(transition<states,lexer_data>(
        states::BASE,
        states::SEEK_DIRECTIVE,
        match_pattern("[.]")
    ));

    // Eat directive - '.data'
    //                   ^^^^
    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_DIRECTIVE,
        states::SEEK_DIRECTIVE,
        match_pattern("[a-zA-Z]"),

        [&](lexer_data& lex) {
            // eat characters
            lex.m_buffer << lex.m_char;
        }
    ));

    // Finish directive - '.data\n' or '.data '
    //                          ^^           ^
    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_DIRECTIVE,
        states::BASE,
        match_pattern("[ ]|[\\n]"),
        [&](lexer_data& lex) -> void {
            lex.push_token(token_type::DIRECTIVE, lex.get_buffer());
            lex.clear_buffer();
        }
    ));

    // Invalid directive '.data./$'
    //                         ^^^
    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_DIRECTIVE,
        states::INVALID_TOKEN,
        not_match_pattern("[a-zA-Z]|[ ]|[\\n]"),
        on_invalid_token
    ));
}

std::optional<std::vector<lexer::token>> lexer::lex(const std::string &input) {
    std::vector<token> output;

    lexer_data lex;


    for(auto& ch : input) {
        lex.m_char = ch;
        m_fsm.tick(lex);
    }

    return lex.m_tokens;
};

}