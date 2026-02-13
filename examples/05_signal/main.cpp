#include "../common.hpp"

#include <vbase/event/signal.hpp>

using namespace vbase;

int main()
{
    print_title("Signal");

    Signal<int> s {};
    auto        c = s.connect([](int v) { std::cout << "v=" << v << "\n"; });

    s.emit(1);

    c.disconnect();

    s.emit(2);

    return 0;
}