// CharState: the abstract base of the character state objects. The vtable is two header words, a
// destructor pair (D1 at slot 0x00, D0 at slot 0x04) and five pure virtual slots (0x08 to 0x18). The
// slot names and signatures are not recovered yet. No data members are recovered for this class.
// Original 32-bit vtable size 0x24 (36). Destructor D0 frees the object with CustomFree.
// Reconstructed from libDungeonHunter2.so pseudocode (dtors 0x3bffdc and 0x3c0684).
#pragma once

#include <cstdint>

class CharState {
public:
    virtual ~CharState();                                               // 0x3bffdc (D1), 0x3c0684 (D0)

    // Pure virtual slots, vtable order. Names and signatures not recovered.
    virtual void unk_vslot_08() = 0;                                    // +0x08 pure
    virtual void unk_vslot_0c() = 0;                                    // +0x0c pure
    virtual void unk_vslot_10() = 0;                                    // +0x10 pure
    virtual void unk_vslot_14() = 0;                                    // +0x14 pure
    virtual void unk_vslot_18() = 0;                                    // +0x18 pure
};
