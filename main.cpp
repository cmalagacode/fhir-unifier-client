#include <process.hpp>

int main(const int argc, char** argv)
{
    std::vector<std::string> args(argv, argv + argc);
    auto display = Proc::Cli(argc, args);
    display.init();
    return 0;
}
