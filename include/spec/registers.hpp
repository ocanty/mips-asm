//
// Created by ocanty on 20/01/19.
//

#ifndef MIPS_ASM_REGISTERS_HPP
#define MIPS_ASM_REGISTERS_HPP

#include <unordered_map>
#include <string>

namespace as::spec {
    const std::unordered_map<std::string, std::uint8_t> registers = {
        { "zero", 0 }, // Zero constant
        { "at", 1 },   // Assembly temporary
        { "v0", 2 },   // Function result
        { "v1", 3 },   // and expression evaluations
        { "a0", 4 },   // Arguments
        { "a1", 5 },
        { "a2", 6 },
        { "a3", 7 },
        { "t0", 8 },   // Temporaries
        { "t1", 9 },
        { "t2", 10 },
        { "t3", 11 },
        { "t4", 12 },
        { "t5", 13 },
        { "t6", 14 },
        { "t7", 15 },
        { "s0", 16 },  // Saved temporaries
        { "s1", 17 },
        { "s2", 18 },
        { "s3", 19 },
        { "s4", 20 },
        { "s5", 21 },
        { "s6", 22 },
        { "s7", 23 },
        { "t8", 24 },  // Temporaries
        { "t9", 25 },
        { "k0", 26 },  // Kernel
        { "k1", 27 },
        { "gp", 28 },  // Global pointer
        { "sp", 29 },  // Stack pointer
        { "fp", 30 },  // Frame pointer
        { "ra", 31 }   // Return address
    };
}

#endif //MIPS_ASM_REGISTERS_HPP
