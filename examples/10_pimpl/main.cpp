#include "../common.hpp"

#include <iostream>
#include <vbase/core/pimpl.hpp>

using namespace vbase;

struct Impl
{
    int v = 0;
};

class Foo
{
public:
    Foo() : m_Impl(std::in_place, Impl {}) {}

    void tick()
    {
        m_Impl->v++;
        std::cout << m_Impl->v << "\n";
    }

private:
    Pimpl<Impl> m_Impl;
};

int main()
{
    print_title("Pimpl");

    Foo f {};

    for (int i = 0; i < 5; i++)
        f.tick();

    return 0;
}