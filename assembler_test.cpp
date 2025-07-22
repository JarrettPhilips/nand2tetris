#include <catch2/catch_test_macros.hpp>
#include "assembler.h"

TEST_CASE("Addition works", "[math]") {
    REQUIRE(is_number("3") == true);
    REQUIRE(is_number("n") == false);
}
