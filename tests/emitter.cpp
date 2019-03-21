
#include <vector>
#include <catch.hpp>
#include <functional>
#include <emitter/emitter.hpp>

#include "emitter/encode.hpp"
#include "lexer/token.hpp"
#include "lexer/token_type.hpp"


std::function<std::optional<std::uint32_t>(const std::vector<as::token>&)>
encode_instruction = [](const std::vector<as::token>& tokens) -> std::optional<std::uint32_t> {
    using namespace as;

    auto ret = as::encode_instruction(tokens, {});
    return ret;
};

TEST_CASE("Emitter encoding - R type", "[emitter]" ) {
    using tk = as::token_type;
    WHEN("sub $8, $8, $9") {
        auto output = encode_instruction({
            { tk::MNEMONIC,       0, "sub" },
            { tk::REGISTER,       0, 8 },
            { tk::COMMA,          0, 0 },
            { tk::REGISTER,       0, 8 },
            { tk::COMMA,          0, 0 },
            { tk::REGISTER,       0, 9 }
        });

        THEN("Correct R encoding") {
            REQUIRE(output.has_value());
            REQUIRE(output.value() == 0x01094022);
        };

    }
}

TEST_CASE("Emitter encoding - J type", "[emitter]" ) {
    using tk = as::token_type;
    WHEN("j <label>") {
        auto output = as::encode_instruction({
            { tk::MNEMONIC, 0, "sub" },
            { tk::LABEL, 0, "ayy"}
        }, { {"ayy", 0x2c} });

        THEN("Correct J encoding") {
            REQUIRE(output.has_value());
            REQUIRE(output.value() == 0x01094022);
        };

    }
}

TEST_CASE("Emitter encoding - I type", "[emitter]" ) {
    using tk = as::token_type;
    WHEN("ori $8, $0, 2") {
        auto output = encode_instruction({
            { tk::MNEMONIC,       0, "ori" },
            { tk::REGISTER,       0, 8 },
            { tk::COMMA,          0, 0 },
            { tk::REGISTER,       0, 0 },
            { tk::COMMA,          0, 0 },
            { tk::LITERAL_NUMBER, 0, 2 }
        });

        THEN("Correct I encoding") {
            REQUIRE(output.has_value());
            REQUIRE(output.value() == 0x34080002);
        };

    }
}
}
