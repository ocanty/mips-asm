//
// Created by ocanty on 27/01/19.
//


#include <algorithm>
#include <cstdint>
#include "emitter.hpp"

#include "../spec/instruction_defs.hpp"
#include "../fsm/transition.hpp"
#include "op_sequences.hpp"

namespace as {

emitter::emitter() :
    m_fsm(INITIAL_STATE) {
    this->setup_fsm();
}

std::optional<std::vector<std::uint8_t>> emitter::emit(const std::vector<token>& tokens) {
    if(tokens.empty()){
        return std::nullopt;
    }

    emitter_context em(tokens.at(0));

    // feed each token into the fsm
    for(auto& tk : tokens) {
        em.set_cur_token(tk);

        m_fsm.tick(em);
    }

    if(em.errors().str().size() > 0) {
        std::cout << em.errors().str() << std::endl;
        return std::nullopt;
    }

    return std::vector<std::uint8_t>();

}

void emitter::setup_fsm() {

    // returns an fsm should_transition_func
    // that transitions if a DIRECTIVE token is read matching the directive name
    auto match_directive = [](const std::string& directive) -> auto {
        return transition<std::size_t, emitter_context>::should_transition_func(
            [=](emitter_context& em) -> bool {
                if(em.cur_token().type() == token_type::DIRECTIVE &&
                    std::get<std::string>(em.cur_token().attribute()) == directive) {

                    // em.set_next_error("Expected labels, instruction or data definition");
                    return true;
                }

                // .set_next_error("Expected a directive, probably: " + directive);
                return false;
            }
        );
    };

    // push current token to buffer
    auto push_buffer = [](emitter_context& em) {
        em.token_buffer().emplace_back(em.cur_token());
    };

    // helper func to clear buffer (fsm callback)
    auto clear_buffer = [](emitter_context& em) {
        em.token_buffer().clear();
    };


    m_fsm.add_transitions({

    // Enter text state when we encounter .text
    {
        INITIAL_STATE,
        TEXT_STATE,
        match_directive("text")
    },

    // Enter data state when we encounter .data
    {
        INITIAL_STATE,
        DATA_STATE,
        match_directive("data")
    },

    // Ignore newlines
    {
        TEXT_STATE,
        TEXT_STATE,
        [](emitter_context& em) -> bool {
            return (em.cur_token().type() == token_type::NEW_LINE);
        }
    },

    // If we encounter a token that isn't a new line, begin seeking the sequence
    {
        TEXT_STATE,
        TEXT_SEEK_UNTIL_NEWLINE,
        [](emitter_context& em) -> bool {
            return (em.cur_token().type() != token_type::NEW_LINE);
        },
        push_buffer
    },

    // Continue seeking if we don't encounter the newline delimiter
    {
        TEXT_SEEK_UNTIL_NEWLINE,
        TEXT_SEEK_UNTIL_NEWLINE,
        [](emitter_context& em) -> bool {
            return (em.cur_token().type() != token_type::NEW_LINE);
        },
        push_buffer
    },


    // If we encountered the new line delimiter, it's time to check the validity
    // of the sequence
    {
        TEXT_SEEK_UNTIL_NEWLINE,
        TEXT_STATE,
        [](emitter_context& em) -> bool {
            return (em.cur_token().type() == token_type::NEW_LINE);
        },

        [](emitter_context& em) {
            for(auto& seq : op_sequences::all()) {
                if(em.token_types_buffer() == seq.token_types()) {
                    em.encode_instruction(seq);

                    em.token_buffer().clear();

                    return;
                }
            }

            em.errors() << "Unknown sequence of tokens ";

            if(em.token_buffer().size() > 0) {
                em.errors() << "near line "
                            << em.token_buffer().at(0).line()
                            << " ";
            }

            em.errors() << std::endl;
        }
    }
    });
}

}