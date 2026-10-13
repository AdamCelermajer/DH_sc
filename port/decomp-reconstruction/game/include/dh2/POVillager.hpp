// POVillager: the physics body of a villager NPC. It adds no state and no collision callbacks of its
// own: the vtable (_ZTV10POVillager, 52 bytes) only replaces the destructor slots, and every other
// slot is POCharacter's. Its layout is therefore POCharacter's layout.
// Original 32-bit size not recovered (no operator new size or constructor symbol).
// Reconstructed from libDungeonHunter2.so pseudocode (dtors 0x3b3824 / 0x3b43e0).
#pragma once

#include "dh2/POCharacter.hpp"

class POVillager : public POCharacter {
public:
    ~POVillager() override;                                         // +0x00/+0x04 0x3b3824 (D1), 0x3b43e0 (D0)
};
