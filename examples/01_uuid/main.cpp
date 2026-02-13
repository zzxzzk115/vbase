#include "../common.hpp"

#include <vbase/core/span.hpp>
#include <vbase/core/uuid.hpp>

using namespace vbase;

int main()
{
    print_title("UUID");

    UUID a = uuid_random();
    UUID b = uuid_random();
    std::cout << "random a = " << to_string(a) << "\n";
    std::cout << "random b = " << to_string(b) << "\n";

    UUID k1 = uuid_from_string_key("player/hero.mesh");
    UUID k2 = uuid_from_string_key("player/hero.mesh");
    std::cout << "k1==k2? " << (k1 == k2 ? "yes" : "no") << "\n";

    std::string s = to_string(k1);
    UUID        parsed {};
    bool        ok = try_parse_uuid(s.c_str(), parsed);
    std::cout << "roundtrip ok? " << (ok && parsed == k1 ? "yes" : "no") << "\n";

    return 0;
}