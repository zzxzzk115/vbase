#include "../common.hpp"

#include <vbase/core/strong_id.hpp>

using namespace vbase;

struct TexTag
{};
using TextureID = StrongID<TexTag, uint32_t>;

int main()
{
    print_title("StrongID");

    TextureID a {1}, b {2};
    std::cout << "a==b? " << (a == b ? "yes" : "no") << "\n";

    TextureID c {1};
    std::cout << "a==c? " << (a == c ? "yes" : "no") << "\n";

    return 0;
}