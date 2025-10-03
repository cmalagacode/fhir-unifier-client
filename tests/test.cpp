#include "process.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Helper Methods/Functions", "[Helper Methods/Functions]")
{
    REQUIRE(Proc::Cli::getNpiIndex("22") == 22);
    REQUIRE(Proc::Cli::getNpiIndex("33") == 33);
    REQUIRE(Proc::Cli::getNpiIndex("0") == 0);
    REQUIRE(Proc::Cli::getNpiIndex("2") == 2);
    REQUIRE(Proc::Cli::getNpiIndex("7") == 7);

    REQUIRE(Proc::Cli::splitTargets("1,2,3,4")
            == std::vector<std::string>{"1", "2", "3", "4"});

}