// POColmap: a PhysicalObject subclass. Only its destructors are recovered, and it adds no members or
// virtuals of its own: its vtable (_ZTV8POColmap, 48 bytes) has the same ten slots as PhysicalObject's,
// and its destructors reset the vptr and call ~PhysicalObject. The original 32-bit size is not recovered
// (the constructor is not in the queue).
// Reconstructed from libDungeonHunter2.so pseudocode (dtor 0x388628 D1, 0x388dcc D0).
#pragma once

#include "dh2/PhysicalObject.hpp"

class POColmap : public PhysicalObject {
public:
    // Vtable slots 0 and 1 (the destructors). Slots 2 to 9 are PhysicalObject's and are not redeclared here.
    ~POColmap() override;                                               // +0x00, +0x04: 0x388628 (D1), 0x388dcc (D0)
};
