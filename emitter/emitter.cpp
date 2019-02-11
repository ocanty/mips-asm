//
// Created by ocanty on 27/01/19.
//


#include <algorithm>
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

        if(m_fsm.tick(em) == ERROR_STATE) {
            std::cout << "Assembling failed " << std::endl;
            return std::nullopt;
        }
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

                    em.set_next_error("Expected labels, instruction or data definition");
                    return true;
                }

                em.set_next_error("Expected a directive, probably: " + directive);
                return false;
            }
        );
    };

    // helper func to clear buffer (fsm callback)
    auto clear_buffer = [](emitter_context& em) -> void {
        em.token_buffer().clear();
    };

    // if no transition is available between a sequence of tokens
    // it means the user supplied incorrect syntax
    // print the last error set
    m_fsm.set_no_transition_available_handler([] (emitter_context& em) {
        std::cout << "Error in sequence near line " << em.cur_token().line() << ": " << std::endl;

        for(auto& token : em.token_buffer()) {
            std::cout << token.name() << " ";
        }

        std::cout << std::endl;
        std::cout << em.get_next_error() << std::endl;

        return ERROR_STATE;
    });

    // Add the transitions that
    // so when we encounter a text or data directive we move into their respective modes
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

    // Add transitions from TEXT_STATE (in the text segment)
    // that match operation token sequences
    // the lamdba below will be called whenever we find tokens
    // matching a possible sequence, (aka an instruction has been found)
    // then it's time to encode it (using the emitter_context)
    for(auto& op_sequence : op_sequences::all()) {

        add_transitions_for_sequence(TEXT_STATE, op_sequence, [=](emitter_context& em) {
            // Try to write an instruction using the sequence we detected
            // the emitter context will use the current token buffer
            if(!em.try_write_instruction(op_sequence)) {
                std::cout << "failed" << std::endl;
            }

        });
    }

//    // add support for newlines
//    add_transitions_for_sequence(TEXT_STATE, op_sequence({t::NEW_LINE}),
//        [=](emitter_context& em) {
//            return;
//        }
//    );
}


// Adds state transitions for a sequence of token types
void emitter::add_transitions_for_sequence(
        const std::size_t& start_state,
        const op_sequence& sequence,
        std::function<void(emitter_context&)> handler) {

    // the sequence is empty, don't add it
    if(sequence.token_types().begin() == sequence.token_types().end()) return;

    // if the sequence consists of only 1 token,
    // we don't need to link states together
    // we can simply do start_state -> start_state instead
    if(sequence.token_types().size() == 1) {
        m_fsm.add_transition({
            start_state,
            start_state,

            [=](emitter_context& em) -> bool {
                em.token_buffer().push_back(em.cur_token());
                // do the transition if the supplied token matches the sequence
                return em.cur_token().type() == sequence.token_types().at(0);
            },

            [=](emitter_context& em) {
                // call their handler
                handler(em);

                // clear the buffer
                em.token_buffer().clear();
            }
        });

        return;
    }

    std::size_t state = start_state;

    // add a transition for each part of the token sequence
    // e.g. for MNEMONIC, REGISTER, REGISTER, CONSTANT
    // we do MNEMONIC -> REGISTER, REGISTER -> REGISTER state transitions here
    for(auto it = sequence.token_types().begin(); it != std::prev(sequence.token_types().end()); ++it) {
        m_fsm.add_transition({
            state,
            m_last_state,
            [=](emitter_context& em) -> bool {

                // Set an error to be displayed if this transition function fails
                // /if the transition does not exist
                em.set_next_error("Expected a token " +
                    get_name_for_token(*it) +
                    " near " +
                    em.cur_token().name()
                );

                // for each sequence transition, we add the token to the buffer
                // so that it's available when the sequence is complete
                em.token_buffer().push_back(em.cur_token());

                // only transition to the next token/sequence transition if the token
                // matches the next one
                return em.cur_token().type() == *it;
            }
        });

        state = m_last_state;
        m_last_state++;
    }

    // Add the final transition
    // e.g. for MNEMONIC, REGISTER, REGISTER, CONSTANT
    // in this case it'l be triggered on REGISTER -> CONSTANT
    // We want to transition to the base state to start
    // finding the next instruction
    auto it = std::prev(sequence.token_types().end());
    m_fsm.add_transition({
        state,
        start_state, // go back to the supplied base state (i.e. usually searching for another instruction)

        [=](emitter_context& em) -> bool {
            // if the transition fails, set the error
            // to tell the user what token we expected
            em.set_next_error("Expected a token " +
                 get_name_for_token(*it) +
                 " near " +
                 em.cur_token().name()
            );


            em.token_buffer().push_back(em.cur_token());
            return em.cur_token().type() == *it;
        },


        // The final token has been found, we need to call their handler
        [=](emitter_context& em) {

            // call it
            handler(em);

            // clear the buffer
            em.token_buffer().clear();
        }
    });
}

}
