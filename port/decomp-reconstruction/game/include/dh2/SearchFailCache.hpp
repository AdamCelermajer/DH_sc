// SearchFailCache: a short list of recent search failures, so a repeated failing search can be skipped.
// It is an STLport vector of Entry, three 4-byte fields each. add() appends an entry, and when more
// than 10 entries are stored it starts over from empty.
// Original 32-bit size 0xc (12): the vector's begin, end and end-of-storage pointers (+0x00, +0x04, +0x08).
// Reconstructed from libDungeonHunter2.so pseudocode (add 0x52a504). Entry field meanings not recovered.
#pragma once

#include <cstdint>
#include <vector>

class SearchFailCache {
public:
    // One cached failure, 12 bytes in the original. The three fields are copied as-is by add().
    struct Entry {
        std::uint32_t unk_00;                                       // +0x00 copied from the argument
        std::uint32_t unk_04;                                       // +0x04 copied from the argument
        std::uint32_t unk_08;                                       // +0x08 copied from the argument
    };

    void add(Entry& entry);                                         // 0x52a504

    // Vector storage: +0x00 begin, +0x04 end, +0x08 end of storage (STLport layout in the original).
    std::vector<Entry> m_entries;                                   // +0x00 .. +0x0b
};
