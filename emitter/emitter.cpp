//
// Created by ocanty on 27/01/19.
//

#include "emitter.hpp"
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

    for(auto& tk : tokens) {
        em.set_token(tk);

        if(m_fsm.tick(em) == ERROR_STATE) {
            return std::nullopt;
        }
    }

    return std::vector<std::uint8_t>();

}

void emitter::setup_fsm() {
    auto match_directive = [](const std::string& directive) -> auto {
        return transition<std::size_t, emitter_context>::should_transition_func(
            [=](emitter_context& em) -> bool {
                if(em.get_token().get_type() == token_type::DIRECTIVE &&
                    std::get<std::string>(em.get_token().get_attribute()) == directive) {

                    em.set_next_error("Expected labels, directives, instruction or data definition");

                    return true;
                }

                em.set_next_error("Expected a directive, probably: " + directive);
                return false;
            }
        );
    };

    // helper func to clear buffer
    auto clear_buffer = [](emitter_context& em) -> void {
        em.token_buffer().clear();
    };

    // if no transition is available between a sequence of tokens
    // it means the user supplied incorrect syntax
    m_fsm.set_no_transition_available_handler([] (emitter_context& em) {
        std::cout << "Error in sequence near line " << em.get_token().get_line() << ": ";

        for(auto& e : em.token_buffer()) {
            std::cout << e.get_name() << " ";
        }

        std::cout << std::endl;
        std::cout << em.get_next_error() << std::endl;

        return ERROR_STATE;
    });

    m_fsm.add_transitions({
    {
        INITIAL_STATE,
        TEXT_STATE,
        match_directive("text")
    },

    {
        INITIAL_STATE,
        DATA_STATE,
        match_directive("data")
    }

    });

    for(auto& seq_kv : op_sequences) {
        auto& seq = seq_kv.second;

        add_transitions_for_sequence(TEXT_STATE, seq, [&seq](emitter_context& em) {

        });
    }
}


// ideally i'd like to have achieved this using template metaprogramming, but maybe in the future
void emitter::add_transitions_for_sequence(
        const std::size_t& start_state,
        const op_sequence& sequence,
        const std::function<void(emitter_context&)> handler) {


    if(sequence.tokens.begin() == sequence.tokens.end()) return;

    std::size_t state = start_state;

    for(auto it = sequence.tokens.begin(); it != sequence.tokens.end(); ++it) {

        // add handler on last iteration
        if(it == sequence.tokens.end()) {
            m_fsm.add_transition({
                state,
                start_state,
                [&](emitter_context& em) -> bool {
                    em.set_next_error("Expected a token near line " + get_name_for_token(*it));
                    return em.get_token().get_type() == *it;
                },

                [&](emitter_context& em) -> void {
                    handler(em);
                    em.token_buffer().clear();
                }
            });
        }

        // nest states for each
        m_fsm.add_transition({
            state,
            m_last_state,
            [&it](emitter_context& em) -> bool {
                em.set_next_error("Expected a token near " + get_name_for_token(*it));
                em.token_buffer().push_back(em.get_token());
                return em.get_token().get_type() == *it;
            }
        });

        state = m_last_state;
        m_last_state++;
    }
}

}
