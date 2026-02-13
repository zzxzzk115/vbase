#include "../common.hpp"

#include <vbase/io/binary_reader.hpp>
#include <vbase/io/binary_writer.hpp>

using namespace vbase;

int main()
{
    print_title("BinaryRW");

    BinaryWriter w {};
    w.write<uint32_t>(123);
    w.write<float>(3.14f);

    auto&        buf = w.bytes();
    BinaryReader r({buf.data(), buf.size()});

    uint32_t v = r.read<uint32_t>();
    std::cout << "value=" << v << "\n";

    float f = r.read<float>();
    std::cout << "value=" << f << "\n";

    return 0;
}