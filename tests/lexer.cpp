//
// Created by ocanty on 13/03/19.
//

#include <catch.hpp>
#include "lexer/lexer.hpp"
#include "lexer/token_type.hpp"

TEST_CASE("Lexer lexes", "[lexer]" ) {

    using namespace as;
    using tk = as::token_type;

    as::lexer lexer;

    WHEN("Nothing passed") {
        auto output = lexer.lex("");

        THEN("Output size 0") {
            REQUIRE(!output.has_value());
        };

    }

    WHEN("Single instruction") {
        auto output = lexer.lex("add\n");

        THEN("Test token\n") {
            REQUIRE(output.value().size() == 2);
            REQUIRE(output.value().at(0).type() == tk::MNEMONIC);
            REQUIRE(output.value().at(1).type() == tk::NEW_LINE);
        };

        THEN("Test attribute\n") {
            REQUIRE(std::get<std::string>(output.value().at(0).attribute()) == "add");
        }

    }
}
