// PolyStat: only its destructor is queued so far. The destructor reads two words: +0x00 is compared
// with the buffer begin (as an end address) and +0x14 is the buffer begin. It frees the buffer unless
// the begin points back at the object. No vtable and no constructor are recovered.
// Original 32-bit size not recovered (no operator new size in class-facts). The destructor is
// 0x5102e8 (D1 only; no D0 or D2 queued).
// Reconstructed from libDungeonHunter2.so pseudocode (dtor 0x5102e8).
#pragma once

#include <cstdint>

class PolyStat {
public:
    ~PolyStat();                                                    // 0x5102e8 (D1)

    // Only the words the destructor reads are named. The bytes between them are not touched by it.
    void* unk_00;                                                   // +0x00 read by the destructor as the end address of the buffer
    uint8_t pad_04[0x10];                                           // +0x04 .. +0x13 not recovered
    void* unk_14;                                                   // +0x14 buffer begin; freed by the destructor unless it equals this
};
