//
// Created by ocanty on 16/01/19.
//

#include <string>
#include <sstream>
#include <regex>
#include "lexer.hpp"
#include "../fsm/transition.hpp"
#include "../spec/instruction_defs.hpp"
#include "../spec/registers.hpp"
#include "token.hpp"

namespace as {

lexer::lexer() :
    m_fsm(states::BASE) {
    setup_fsm();
}

void lexer::setup_fsm() {
    // Transition callback that will simply consume the character into the character buffer
    auto consume_char = [&](lexer_context& lex) {
        lex.consume(lex.ch());
    };

    // Transition callback generator that pushes an
    // invalid_token to the token buffer for a given reason
    auto push_invalid_token = [](const std::string& reason) -> auto {
        return transition<states, lexer_context>::transition_callback_func(
            [&reason](lexer_context& lex) -> void {
                // Push invalid token with error reason
                lex.push_token(
                    token_type::INVALID_TOKEN,
                    reason            +
                    " near '"         +
                    lex.char_buffer().str() +
                    "' at line "      +
                    std::to_string(lex.cur_line())
                );

                lex.clear_char_buffer();
            }
        );
    };

    // Generate's a should_transition_func
    // that will return true when a char matches the supplied regex
    auto match_pattern = [](const std::string& regex) -> auto {
        return transition<states, lexer_context>::should_transition_func(
            [=](lexer_context& lex) -> bool {
                return std::regex_match(std::string(1,lex.ch()), std::regex(regex));
            }
        );
    };

    // the above, just not'ed
    auto not_match_pattern = [](const std::string& regex) -> auto {
        return transition<states, lexer_context>::should_transition_func(
            [regex](lexer_context& lex) -> bool {
                return !std::regex_match(std::string(1,lex.ch()), std::regex(regex));
            }
        );
    };

    // Parses a register from a register string, expects the $ to be removed
    // i.e. valid inputs -> "31", "02", "t0", etc...
    // as specified in spec::registers
    auto get_register_id = [](const std::string& lexeme) -> std::optional<std::uint8_t> {
        // check if it's a number
        try {
            int i_dec = std::stoi(lexeme);

            if (i_dec > 0 && i_dec < 32) {
                return i_dec;
            }
        }
        catch(const std::invalid_argument& e) {
            // check if its a named register (if its not a number)
            if(spec::registers.count(lexeme)) {
                return spec::registers.at(lexeme);
            }
        }

        return std::nullopt;
    };

    m_fsm.add_transitions(
    {

    // Comma
    {
        states::BASE, states::BASE,
        match_pattern("[,]"),
        [&](lexer_context& lex) -> void {
            lex.push_token(token_type::COMMA);
        }
    },

    {
        states::BASE,
        states::BASE,
        match_pattern("[\\n]"),
        [&](lexer_context& lex) -> void {
            lex.push_token(token_type::NEW_LINE);
        }
    },
        
    // Begin directive - '.data'
    {
        states::BASE,
        states::SEEK_DIRECTIVE,
        match_pattern("[\\.]")
    },

    // Eat directive - '.data'
    //                   ^^^^
    { states::SEEK_DIRECTIVE, states::SEEK_DIRECTIVE,
        match_pattern("[a-zA-Z]"),
        consume_char
    },
    
    // Finish directive - '.data\n' or '.data '
    //                          ^^           ^
    {
        states::SEEK_DIRECTIVE,
        states::BASE,
        match_pattern("[ ]|[\\n]"),
        [&](lexer_context& lex) -> void {
            lex.push_token(token_type::DIRECTIVE, lex.char_buffer().str());
            lex.clear_char_buffer();

            if(lex.ch() == '\n') {
                lex.push_token(token_type::NEW_LINE);
            }
        }
    },

    // Invalid directive '.data./$'
    //                         ^^^
    {
        states::SEEK_DIRECTIVE,
        states::INVALID_TOKEN,
        not_match_pattern("[a-zA-Z]|[ ]|[\\n]"),
        push_invalid_token("Invalid directive characters")
    },

    // Begin label or mnemonic - 'mov ' or 'mov\n' or 'movie_label ' or 'movie_label\n'
    //                            ^         ^          ^                 ^
    // We determine if it's a label or instruction when we reach the terminating character
    // i.e the space or new line
    {
        states::BASE,
        states::SEEK_LABEL_OR_MNEMONIC,
        match_pattern("[_a-zA-Z]"),
        consume_char
    },

    // Eat label or mnemonic - 'mov ' or 'mov\n' or 'movie_label ' or 'movie_label\n'
    //                           ^^        ^^         ^^^^^^^^^^        ^^^^^^^^^^
    {
        states::SEEK_LABEL_OR_MNEMONIC,
        states::SEEK_LABEL_OR_MNEMONIC,
        match_pattern("[_a-zA-Z0-9]"),
        consume_char
    },

    // Finish label - 'mov ' or 'mov\n' or 'movie_label ' or 'movie_label\n'
    //                    ^         ^                  ^                 ^
    // Here we need to check if it's a instruction first,
    // and if it isn't it's a label
    {
        states::SEEK_LABEL_OR_MNEMONIC,
        states::BASE,
        match_pattern("[ ]|[\\n]"),
        [&](lexer_context &lex) -> void {
            auto char_buffer = lex.char_buffer().str();

            // if we have an instruction that matches the char buffer
            if (spec::instructions::exists(char_buffer)) {
                lex.push_token(token_type::MNEMONIC, char_buffer);
                lex.clear_char_buffer();
            } else {
                // it's a label
                lex.push_token(token_type::LABEL, char_buffer);
                lex.clear_char_buffer();
            }

            if(lex.ch() == '\n') {
                lex.push_token(token_type::NEW_LINE);
            }
        }
    },

    // If a semi-colon follows the label, it's actually a label definition
    //
    {
        states::SEEK_LABEL_OR_MNEMONIC,
        states::BASE,
        match_pattern("[:]"),
        [&](lexer_context& lex) -> void {
            auto char_buffer = lex.char_buffer().str();

            // can't use an instruction as a label definition
            if(spec::instructions::exists(char_buffer)) {
                return push_invalid_token("Using a reserved keyword as a label definition")(lex);
            } else {
                // it's a label
                lex.push_token(token_type::LABEL_DEFINITION, char_buffer);
                lex.clear_char_buffer();
            }
        }
    },

    // Invalid directive '.data./$'
    //                         ^^^
    {
        states::SEEK_DIRECTIVE,
        states::INVALID_TOKEN,
        not_match_pattern("[a-zA-Z]|[ ]|[\\n]"),
        push_invalid_token("Invalid directive characters")
    },

    // Comments, we ignore till newline
    {
        states::BASE,
        states::SEEK_COMMENT,
        match_pattern("[#]")
    },

    // matches everything inside a comment
    {
        states::SEEK_COMMENT,
        states::SEEK_COMMENT,
        match_pattern("[.]")
    },

    // exit comment seeking on newline
    {
        states::SEEK_COMMENT,
        states::BASE,
        match_pattern("[\\n]")
    },

    // Register $reg
    //          ^
    {
        states::BASE,
        states::SEEK_REGISTER,
        match_pattern("[\\$]")
    },

    {
        // seek each character of the register declaration
        states::SEEK_REGISTER,
        states::SEEK_REGISTER,
        match_pattern("[a-z0-9]"),
        consume_char
    },

    // When we reach the end of a register declaration
    // delimited by "," or " " or "\n"
    {
        states::SEEK_REGISTER,
        states::BASE,
        match_pattern("[,| |\\n]"),
        [&](lexer_context &lex) {
            auto char_buffer = lex.char_buffer().str();
            auto reg = get_register_id(char_buffer);

            if(reg != std::nullopt) {
                lex.push_token(token_type::REGISTER, reg.value());
                lex.clear_char_buffer();
            }
            else {
                return push_invalid_token("Invalid register")(lex);
            }

            if(lex.ch() == ',') {
                lex.push_token(token_type::COMMA);
            }

            if(lex.ch() == '\n') {
                lex.push_token(token_type::NEW_LINE);
            }
        }
    },

    {
        states::BASE,
        states::SEEK_LITERAL_STRING,
        match_pattern("[\"]")
    },

    {
        states::SEEK_LITERAL_STRING,
        states::SEEK_LITERAL_STRING,
        not_match_pattern("[\"]"),
        consume_char
    },


    {
        states::SEEK_LITERAL_STRING,
        states::BASE,
        match_pattern("[\"]"),
        [&](lexer_context &lex) {
            auto char_buffer = lex.char_buffer().str();
            lex.push_token(token_type::LITERAL_STRING, char_buffer);
            lex.clear_char_buffer();
        }
    },


    {
        states::BASE,
        states::SEEK_LITERAL_CHAR,
        match_pattern("[']")
    },

    {
        states::SEEK_LITERAL_CHAR,
        states::SEEK_LITERAL_CHAR,
        not_match_pattern("[']"),
        consume_char
    },


    {
        states::SEEK_LITERAL_CHAR,
        states::BASE,
        match_pattern("[']"),
        [&](lexer_context &lex) {
            auto char_buffer = lex.char_buffer().str();

            if(char_buffer.size() > 1) {
                return push_invalid_token("Invalid character literal")(lex);
            }

            lex.push_token(token_type::LITERAL_CHAR, char_buffer);
            lex.clear_char_buffer();
        }
    },


    {
        states::BASE,
        states::SEEK_LITERAL_NUMBER,
        match_pattern("[\\-|0-9|\\+]"),
        consume_char
    },

    {
        states::SEEK_LITERAL_NUMBER,
        states::SEEK_LITERAL_NUMBER,
        match_pattern("[0-9]"),
        consume_char
    },

    {
        states::SEEK_LITERAL_NUMBER,
        states::BASE,
        match_pattern("[ |\\n]"),
        [&](lexer_context &lex) {
             auto char_buffer = lex.char_buffer().str();

             try {
                 int i_dec = std::stoi(char_buffer);
                 lex.push_token(token_type::LITERAL_NUMBER,i_dec);
                 lex.clear_char_buffer();
             }
             catch(const std::exception& e) {
                return push_invalid_token("Invalid number literal")(lex);
             }

             if(lex.ch() == '\n') {
                lex.push_token(token_type::NEW_LINE);
             }
        }
    },

    {
        states::SEEK_LITERAL_NUMBER,
        states::SEEK_IMM_REG_PRE,
        match_pattern("[(]"),
        [&](lexer_context &lex) {
             auto char_buffer = lex.char_buffer().str();

             try {
                 int i_dec = std::stoi(char_buffer);
                 lex.push_token(token_type::OFFSET, i_dec);
             }
             catch(const std::exception& e) {
                return push_invalid_token("Invalid number literal")(lex);
             }
        }
    },

    {
        states::SEEK_IMM_REG_PRE,
        states::SEEK_IMM_REG,
        match_pattern("[$]")
    },

    {
        states::SEEK_IMM_REG,
        states::SEEK_IMM_REG,
        match_pattern("[a-zA-Z]"),
        [&](lexer_context &lex) {
            lex.consume(lex.ch());
        }
    },

    {
        states::SEEK_IMM_REG,
        states::BASE,
        match_pattern("[)]"),
        [&](lexer_context &lex) {
            auto char_buffer = lex.char_buffer().str();
            auto reg = get_register_id(char_buffer);

            if(reg != std::nullopt) {
                lex.push_token(token_type::BASE_REGISTER, reg.value());
                lex.clear_char_buffer();
            } else {
                return push_invalid_token("Invalid register")(lex);
            }
        }
    }


    });
}

std::optional<std::vector<token>> lexer::lex(const std::string &input) {

    lexer_context lex;

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
        lex.set_cur_line(cur_line);

        // pass each char to fsm
        for(auto& ch : cur_line_str) {
            lex.set_ch(ch);
            m_fsm.tick(lex);

            // if an invalid token ever gets pushed
            if(!lex.tokens().empty() &&
                    lex.tokens().end()->type() == token_type::INVALID_TOKEN) {

                // display the error string in the invalid_token token attribute
                std::cout << std::get<std::string>(lex.tokens().end()->attribute()) << std::endl;
                return std::nullopt;
            };
        }

        cur_line++;
    }

    return lex.tokens();
};

}