#include "process.hpp"
#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>

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

TEST_CASE("JSON", "[CLI]")
{
    nlohmann::json test1{};
    test1 = nlohmann::json::parse("[{\"code\":\"101YP2500X\",\"desc\":\"Counselor, Professional\"}]");
    REQUIRE(test1.size() == 1);
    REQUIRE(test1.dump() == "[{\"code\":\"101YP2500X\",\"desc\":\"Counselor, Professional\"}]");

    nlohmann::json test2{};
    test2 = nlohmann::json::parse("[]");
    REQUIRE(test2.size() == 0);
    REQUIRE(test2.dump() == "[]");
}