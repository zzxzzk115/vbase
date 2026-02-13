#include "../common.hpp"

#include <vbase/core/scope_exit.hpp>

using namespace vbase;

int main()
{
    print_title("ScopeExit");

    int x = 0;
    {
        auto g = scope_exit([&] { std::cout << "exit x=" << x << "\n"; });
        x      = 42;
    }

    return 0;
}