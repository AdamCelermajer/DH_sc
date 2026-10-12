#include "net-transport/net_struct_interpolation.hpp"

#include <cstdio>

int main() {
    // Construct and destroy both instantiations through their base pointers.
    NetStructMember* a = new NetStructInterpolatedRangedFloat18();
    NetStructMember* b = new NetStructInterpolatedRangedFloat16();
    delete a;
    delete b;
    std::printf("net_struct_interpolation: ok\n");
    return 0;
}
