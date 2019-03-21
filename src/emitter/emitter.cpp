//
// Created by ocanty on 09/02/19.
//

#include "emitter/emitter.hpp"
#include "emitter/encode.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <memory>

#include <elf.h>

namespace as {

std::optional<std::vector<std::uint8_t>>
emit(const std::vector<as::token> &tokens) {

    constexpr std::uint32_t SECTION_TEXT_BASE = 0x00000000004000f0;

//    Elf32_Ehdr elf_header = {
//        .e_ident = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3, ELFCLASS32, ELFDATA2MSB, EV_CURRENT, ELFOSABI_SYSV, 0,
//                    0, 0, 0, 0, 0, 0, 0 },
//        .e_type = ET_EXEC,
//        .e_machine = EM_MIPS,
//        .e_version = EV_CURRENT,
//        .e_entry = SECTION_TEXT_BASE,
//        .e_phoff = sizeof(Elf32_Ehdr),
//        .e_shoff = 0,
//        .e_flags = 0,
//        .e_ehsize = sizeof(Elf32_Ehdr),
//
//
//    };

    enum assembly_mode {
        TEXT,
        DATA
    };

    enum assembly_pass {
        PASS_ONE,   // we use this pass to determine where to place code blocks, all labels in this default to 0
        PASS_TWO    // the second pass rewrites the labels to point to where we placed the previous code blocks
    };

    assembly_mode mode = TEXT;
    assembly_pass pass = PASS_ONE;

    std::unordered_map<std::string, std::uint32_t> labels;
    std::vector<token> token_buffer;

    for(auto& tk : tokens) {

//        // if directive encountered
//        if(tk.type() == token_type::DIRECTIVE) {
//            auto& directive = std::get<std::string>(tk.attribute());
//
//            // swap to respective modes
//            if(directive == "text") {
//                mode = TEXT;
//                break;
//            }
//
//            if(directive == "data") {
//                mode = DATA;
//                break;
//            }
//
//            continue;
//        }

        // keep fetching tokens until we encounter the end of a sequence
        if(tk.type() != token_type::NEW_LINE) {
            token_buffer.emplace_back(tk);
        }
        else { // if we encountered the end of a sequence of tokens (delimited by new lines)
            switch(mode) {
                case TEXT:



                break;

                case DATA:
                break;
            }
        }

    }

    return std::nullopt;
}

/**
 *     if(m_text_labels.count(label)) {
        std::cout << "warning: label redefinition, the new label will be used instead ("
                  << label
                  << ") "
                  << std::endl;
    }

 */
}