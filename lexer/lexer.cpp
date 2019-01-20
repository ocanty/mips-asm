//
// Created by ocanty on 16/01/19.
//

#include <string>
#include <sstream>
#include <regex>
#include "lexer.hpp"
#include "../fsm/transition.hpp"
#include "../spec/instructions.hpp"
#include "token.hpp"

namespace as {

lexer::lexer() :
    m_fsm(states::BASE) {
    setup_fsm();
}

void lexer::setup_fsm() {
    // Generates a transition_callback_func that pushes an invalid_token for a given reason
    auto on_invalid_token = [](const std::string& reason) -> auto {
        return transition<states, lexer_data>::transition_callback_func(
            [reason](lexer_data& lex) -> void {
                // Push invalid token with error reason
                lex.push_token(
                    token_type::INVALID_TOKEN,
                    reason           +
                    " near '"        +
                    lex.get_buffer() +
                    "' at line "     +
                    std::to_string(lex.get_line())
                );

                lex.clear_buffer();
            }
        );
    };

    // Generate's a should_transition_func
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
    //                    ^
    m_fsm.add_transition(transition<states,lexer_data>(
        states::BASE,
        states::SEEK_DIRECTIVE,
        match_pattern("[\\.]")
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
        on_invalid_token("Invalid directive characters")
    ));

    // Begin label or mnemonic - 'mov ' or 'mov\n' or 'movie_label ' or 'movie_label\n'
    //                            ^         ^          ^                 ^
    // We determine if it's a label or instruction when we reach the terminating character
    // i.e the space or new line
    m_fsm.add_transition(transition<states,lexer_data>(
        states::BASE,
        states::SEEK_LABEL_OR_MNEMONIC,
        match_pattern("[a-zA-Z]"),
        [&](lexer_data& lex) {
            // eat first character
            lex.m_buffer << lex.m_char;
        }
    ));

    // Eat label or mnemonic - 'mov ' or 'mov\n' or 'movie_label ' or 'movie_label\n'
    //                           ^^        ^^         ^^^^^^^^^^        ^^^^^^^^^^
    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_LABEL_OR_MNEMONIC,
        states::SEEK_LABEL_OR_MNEMONIC,
        match_pattern("[a-zA-Z]"),

        [&](lexer_data& lex) {
            // eat characters
            lex.m_buffer << lex.m_char;
        }
    ));

    // Finish label - 'mov ' or 'mov\n' or 'movie_label ' or 'movie_label\n'
    //                    ^         ^                  ^                 ^
    // Here we need to check if it's a instruction first,
    // and if it isn't it's a label
    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_LABEL_OR_MNEMONIC,
        states::BASE,
        match_pattern("[ ]|[\\n]"),
        [&](lexer_data& lex) -> void {

            // if we have an instruction that matches the char buffer
            // it ain't a label
            if(spec::instructions.at(lex.get_buffer())) {
                lex.push_token(token_type::MNEMONIC, lex.get_buffer());
                lex.clear_buffer();
            } else {
                // it's a label
                lex.push_token(token_type::LABEL, lex.get_buffer());
                lex.clear_buffer();
            }
        }
    ));

    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_LABEL_OR_MNEMONIC,
        states::BASE,
        match_pattern("[:]"),
        [&](lexer_data& lex) -> void {
            // can't use an instruction as a label definition
            if(spec::instructions.at(lex.get_buffer())) {
                return on_invalid_token("Using a reserved keyword as a label definition")(lex);
            } else {
                // it's a label
                lex.push_token(token_type::LABEL_DEFINITION, lex.get_buffer());
                lex.clear_buffer();
            }
        }
    ));

    // Invalid directive '.data./$'
    //                         ^^^
    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_DIRECTIVE,
        states::INVALID_TOKEN,
        not_match_pattern("[a-zA-Z]|[ ]|[\\n]"),
        on_invalid_token("Invalid directive characters")
    ));

    // Comments, we ignore till newline
    m_fsm.add_transition(transition<states,lexer_data>(
        states::BASE,
        states::SEEK_COMMENT,
        match_pattern("[#]")
    ));

    // matches everything
    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_COMMENT,
        states::SEEK_COMMENT,
        match_pattern("[.]")
    ));

    // exit comment seeking on newline
    m_fsm.add_transition(transition<states,lexer_data>(
        states::SEEK_COMMENT,
        states::BASE,
        match_pattern("[\\n]")
    ));


}

std::optional<std::vector<token>> lexer::lex(const std::string &input) {

    lexer_data lex;

    // create a stringstream so we can read line by line below
    // the newline we add is a cautionary measure to ensure the last statement
    // finishes on a newline rather than EOF
    std::stringstream ss(input + '\n');
    std::size_t cur_line = 0;
    std::string cur_line_str;

    // we tokenize line by line as it allow the lexer to tell us what
    // errors happened on what line
    while(std::getline(ss,cur_line_str,'\n')) {

        // add back the newline we removed, it's important
        // the lexer uses it to determine ends of comments, statements, etc...
        cur_line_str += '\n';

        // tell lex current line
        lex.m_line = cur_line;

        // pass each char to fsm
        for(auto& ch : cur_line_str) {
            lex.m_char = ch;
            m_fsm.tick(lex);
        }

        cur_line++;
    }

    return lex.m_tokens;
};

}