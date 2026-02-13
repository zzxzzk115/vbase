#include "../common.hpp"

#include <vbase/core/result.hpp>

using namespace vbase;

enum class Err
{
    eDivideByZero
};

Result<int, Err> divide(int a, int b)
{
    if (b == 0)
        return Result<int, Err>::err(Err::eDivideByZero);
    return Result<int, Err>::ok(a / b);
}

int main()
{
    print_title("Result");

    auto r = divide(10, 2);
    if (r)
        std::cout << "10/2=" << r.value() << "\n";

    auto r2 = divide(10, 0);
    if (!r2)
    {
        if (r2.error() == Err::eDivideByZero)
            std::cout << "divide by zero error\n";
        else
            std::cout << "other error\n";
    }

    return 0;
}