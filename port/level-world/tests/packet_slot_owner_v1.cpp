#include "../object_manager_runtime_owner_v1.hpp"
#include <cassert>
#include <iostream>

int main() {
    dh2::packet_slot_owner_v1::Owner packets;
    dh2::object_manager_runtime_owner_v1::Owner objects;
    using dh2::packet_slot_owner_v1::Callbacks;
    assert(objects.network_initialized_1ac() == 0);
    assert(packets.register_slot(1, Callbacks{11,12,13,14}));
    objects.network_init_level(packets, false, {});
    assert(objects.network_initialized_1ac() == 1);
    assert(packets.bitmap() == 2 && !packets.slot(3)->registered);
    objects.network_uninit_level(packets);
    assert(objects.network_initialized_1ac() == 0 && packets.bitmap() == 2);
    objects.network_init_level(packets, true, Callbacks{31,32,33,34});
    assert(packets.bitmap() == 10 && packets.slot(3)->callbacks.read == 32);
    objects.network_init_level(packets, true, Callbacks{41,42,43,44});
    assert(packets.slot(3)->callbacks.read == 32);
    objects.network_uninit_level(packets);
    assert(!packets.slot(3)->registered && !packets.slot(3)->callbacks.write);
    assert(!packets.slot(3)->callbacks.read && !packets.slot(3)->callbacks.acknowledged);
    assert(!packets.slot(3)->callbacks.lost && packets.bitmap() == 2);
    assert(packets.slot(1)->callbacks.read == 12);
    objects.network_uninit_level(packets);
    assert(packets.bitmap() == 2 && !objects.network_initialized_1ac());
    assert(!packets.register_slot(8, {}) && !packets.unregister_slot(8));
    assert(!packets.slot(8) && packets.bitmap() == 2);
    std::cout << "PASS packet slots and ObjectManager network lifecycle\n";
}
