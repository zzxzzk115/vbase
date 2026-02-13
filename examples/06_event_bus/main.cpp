#include "../common.hpp"

#include <vbase/event/event_bus.hpp>

using namespace vbase;

struct Spawn
{
    int id;
};

int main()
{
    print_title("EventBus");

    EventBus bus {};
    auto     sub = bus.subscribe<Spawn>([](const Spawn& e) { std::cout << "spawn " << e.id << "\n"; });

    bus.publish(Spawn {7});

    sub.reset(); // unsubscribe

    bus.publish(Spawn {8}); // no output

    return 0;
}