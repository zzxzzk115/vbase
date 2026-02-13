#include "../common.hpp"

#include <vbase/patterns/singleton.hpp>

using namespace vbase;

class Config : public Singleton<Config>
{
public:
    Config(int v = 0) : v(v) {}

    int v = 0;
};

int main()
{
    print_title("Singleton");

    Config c {520};
    std::cout << Config::instance().v << "\n";

    return 0;
}