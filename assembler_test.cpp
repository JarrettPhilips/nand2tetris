#include <catch2/catch_test_macros.hpp>
#include "assembler.h"

TEST_CASE("Check is_number", "[helper]") {
    REQUIRE(is_number("3") == true);
    REQUIRE(is_number("n") == false);
}

TEST_CASE("Check assemble_a_instruction", "[assemble]") {
    std::string result = assemble_a_instruction("@2");
    std::string expected = "0000000000000010";
    CAPTURE(result, expected);
    CHECK(result == expected);
}

TEST_CASE("Check assemble_c_instruction", "[assemble]") {
    initialize_tables();
    std::string result = assemble_c_instruction("D=D+A");
    std::string expected = "1110000010010000";
    CAPTURE(result, expected);
    CHECK(result == expected);

    result = assemble_c_instruction("D=D+M");
    expected = "1111000010010000";
    CAPTURE(result, expected);
    CHECK(result == expected);

    result = assemble_c_instruction("M=D");
    expected = "1110001100001000";
    CAPTURE(result, expected);
    CHECK(result == expected);

    result = assemble_c_instruction("AM=M-1");
    expected = "1111110010101000";
    CAPTURE(result, expected);
    CHECK(result == expected);
    
    result = assemble_c_instruction("M=!M");
    expected = "1111110001001000";
    CAPTURE(result, expected);
    CHECK(result == expected);

    result = assemble_c_instruction("M=D|M");
    expected = "1111010101001000";
    CAPTURE(result, expected);
    CHECK(result == expected);

    result = assemble_c_instruction("D;JGE");
    expected = "1110001100000011";
    CAPTURE(result, expected);
    CHECK(result == expected);

    result = assemble_c_instruction("0;JMP");
    expected = "1110101010000111";
    CAPTURE(result, expected);
    CHECK(result == expected);
}

TEST_CASE("Check handle symbol", "[symbol]"){
    initialize_tables();

    handle_label("END_EQ", 15);
    std::string result = assemble_a_instruction("@END_EQ");
    std::string expected = "0000000000001111";
    CAPTURE(result, expected);
    CHECK(result == expected);

    // assemble_a_instruction("@ponggame.0");
    result = assemble_a_instruction("@ponggame.0");
    expected = "0000000000010000";
    CAPTURE(result, expected);
    CHECK(result == expected);
}
