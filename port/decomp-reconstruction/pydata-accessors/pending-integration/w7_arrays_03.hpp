#pragma once

// Arrays::X::read(IStreamBase*) for the nine tables of slice arrays_03 that have a readable element
// class in the port (see NOTES.md, "Arrays element ABI"). Each table keeps the static `members`
// (element array) and `size`, and its finalize() runs before the new elements are read.
//
// Stride: members is a typed array, so the element stride is sizeof(Element). The original listing
// stride (for example 72 for Projectile) is not used. This is the same layout decision as the
// slice-00 and slice-01 files, which use listing strides; see the report for that mismatch.
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"
#include "pydata-accessors/structs_records.hpp"

namespace Arrays {

// Original body shared by the reads in this slice: count = readAs<uint32>, finalize(), size = count,
// `count` new elements (the vptr and zeroed words are the element's constructor), then each element's
// virtual read (vtable +0xc) in order, with the same stream.
template <class Element>
void ReadElementArray(Element*& members, std::uint32_t& size, IStreamBase* stream, void (*finalize)()) {
    const std::uint32_t count = StreamReader::readAs<std::uint32_t>(stream);
    finalize();
    size = count;
    members = new Element[count];
    for (std::uint32_t i = 0; i < size; ++i) {
        members[i].read(stream);
    }
}

struct ProjectileTable {
    static Structs::Projectile* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::ProjectileTable::finalize, 0x4a5800 (not in this slice)
    static void read(IStreamBase* stream);
};
struct HintPages {
    static Structs::HintPage* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::HintPages::finalize (not in this slice)
    static void read(IStreamBase* stream);
};
struct HelpPages {
    static Structs::HelpPage* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::HelpPages::finalize (not in this slice)
    static void read(IStreamBase* stream);
};
struct TriggerPlates {
    static Structs::TriggerPlate* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::TriggerPlates::finalize (not in this slice)
    static void read(IStreamBase* stream);
};
struct LiftableObjects {
    static Structs::LiftableObject* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::LiftableObjects::finalize (not in this slice)
    static void read(IStreamBase* stream);
};
struct GameObjectDamager {
    static Structs::GameObjectDamage* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::GameObjectDamager::finalize (not in this slice)
    static void read(IStreamBase* stream);
};
struct FontPalette {
    static Structs::FontColorDef* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::FontPalette::finalize (not in this slice)
    static void read(IStreamBase* stream);
};
struct CharEffectTable {
    static Structs::CharEffect* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::CharEffectTable::finalize (not in this slice)
    static void read(IStreamBase* stream);
};
struct GameOptionTable {
    static Structs::GameOption* members;
    static std::uint32_t size;
    static void finalize();  // Arrays::GameOptionTable::finalize (not in this slice)
    static void read(IStreamBase* stream);
};

}  // namespace Arrays
